/*==============================================================================
Filename:
Project :
Author  :
Version :
Purpose
--------------------------------------------------------------------------------

Version History
--------------------------------------------------------------------------------

License
--------------------------------------------------------------------------------
The copyrights, all other intellectual and industrial property rights are
retained by XMOS and/or its licensors.
Terms and conditions covering the use of this code can be found in the Xmos
End User License Agreement.

Copyright XMOS Ltd 2011

In the case where this code is a modification of existing code under a
separate license, the separate license terms are shown below. The
modifications to the code are still covered by the copyright notice above.
==============================================================================*/

/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "modbus_tcp.h"
#include "xmodbus_transaction.h"

/*------------------------------------------------------------------------------
Extern Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Manifest constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Typedefs
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Global Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Static Variables
------------------------------------------------------------------------------*/
static uint16_t transid;

/*------------------------------------------------------------------------------
Prototypes
------------------------------------------------------------------------------*/
static void mbtcp_init();

static uint16_t mbtcp_get_transid();

static void mbtcp_receive_adu(chanend c_xtcp,
                              xtcp_connection_t *conn,
                              chanend c_user);

static uint8_t mbtcp_parse_adu(s_modbus_tcp_adu_t *adu);

static uint8_t mb_check_mbap(s_modbus_tcp_adu_t *mbap);

static uint8_t mb_check_illegal_function(uint8_t function_code);

static uint8_t mb_check_illegal_data_address_and_value(uint16_t address,
                                                        uint16_t size);

static void mbtcp_send_adu(chanend c_xtcp,
                           xtcp_connection_t *conn,
                           chanend c_user);

static void mbtcp_init_state(chanend c_xtcp, xtcp_connection_t *conn);

/*------------------------------------------------------------------------------
Implementation
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
------------------------------------------------------------------------------*/
static void mbtcp_init()
{
//   uint16_t index;
   transid = 0u;
//
//   adu[INDEX_HI_TRANSACTION_ID]    = 0u;
//   adu[INDEX_HI_TRANSACTION_ID + 1u] = 0u;
//   adu[INDEX_HI_PROTOCOL_ID]       = (MODBUS_PROTOCOL_IDENTIFIER >> 8u) & 0x0Fu;
//   adu[INDEX_HI_PROTOCOL_ID + 1u]  = MODBUS_PROTOCOL_IDENTIFIER  & 0x0Fu;
//   adu[INDEX_HI_LENGTH_FIELD]      = 0u;
//   adu[INDEX_HI_LENGTH_FIELD + 1u] = 0u;
//   adu[INDEX_UNIT_ID]              = MODBUS_UNIT_IDENTIFIER;
//
//   for(index = INDEX_FUNCTION_CODE; index < SIZE_MODBUS_ADU; index++)
//   {
//      adu[index] = 0u;
//   }
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
------------------------------------------------------------------------------*/
static uint16_t mbtcp_get_transid()
{
   return transid++;
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
------------------------------------------------------------------------------*/
static void mbtcp_receive_adu(chanend c_xtcp,
                              xtcp_connection_t *conn,
                              chanend c_user)
{
   s_modbus_tcp_adu_t adu;

   uint16_t data_length;
   uint8_t check_adu;
   uint16_t index;

   data_length = (uint16_t)(xtcp_recv(c_xtcp, adu) - SIZE_MODBUS_MBAP);
   check_adu = mbtcp_parse_adu();

   if(check_adu == ALL_OK)
   {
      xmodbus_receive(c_user, adu);

      xtcp_init_send(c_xtcp, conn);
   }
   else
   {
      // send level 1 exception
   }
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
------------------------------------------------------------------------------*/
static uint8_t mbtcp_parse_adu(s_modbus_tcp_adu_t *adu)
{
   uint8_t check_mbap;
   uint8_t check_illegal_function;
   uint8_t check_illegal_data_address_and_value;

   uint8_t function_code;
   uint16_t address;
   uint16_t size;

   check_mbap = mb_check_mbap(adu);

   if(check_mbap == ALL_OK)
   {
      // MBAP OK
      function_code = adu.function_code;
      check_illegal_function = mb_check_illegal_function(function_code);

      if(check_illegal_function == ALL_OK)
      {
         // Function code supported
         // Object specific
//         address = (uint16_t)((uint16_t)(adu[INDEX_START_DATA] << 8u) +
//                              (uint16_t)(adu[INDEX_START_DATA + 1u]));
//
//         size = (uint16_t)((uint16_t)(adu[INDEX_START_DATA + 2u] << 8u) +
//                           (uint16_t)(adu[INDEX_START_DATA + 3u]));
//
//         check_illegal_data_address_and_value =
//            mb_check_illegal_data_address_and_value(address, size);
//
//         if(check_illegal_data_address_and_value == ALL_OK)
//         {
            // Initial checks OK. Perform required action
            return ALL_OK; // or error returned by processing
//         }
//         else
//         {
//            // Data or address invalid
//            return check_illegal_data_address_and_value;
//         }
      }
      else
      {
         // Function code not supported
         return check_illegal_function;
      }
   }
   else
   {
      // MBAP error
      return check_mbap;
   }
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Notes:

Return values: CAUTION: currently hard coded
ALL_OK = all OK
1 = Transaction identifier mismatch
2 = Protocol identifier mismatch
3 = Length field too long (260 - 2(TxID) - 2(PID) - 2(LengthF) = 254)
------------------------------------------------------------------------------*/
static uint8_t mb_check_mbap(s_modbus_tcp_adu_t *mbap)
{
   uint16_t tid = mbap.transaction_id;

   uint16_t pid = mbap.protocol_id;

   uint16_t len = mbap.length_field;

   if(tid == mbtcp_get_transid())
   {
      // Transaction identifier OK
      if(pid == MODBUS_PROTOCOL_IDENTIFIER)
      {
         // Protocol identifier OK
         if(len <= 254)
         {
            // Length field OK
            return ALL_OK;
         }
         else
         {
            // Length field too long (260 - 2(TxID) - 2(PID) - 2(LengthF) = 254)
            return 3;
         }
      }
      else
      {
         // Protocol identifier mismatch
         return 2;
      }
   }
   else
   {
      // Transaction identifier mismatch
      return 1;
   }
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
Return value:
ALL_OK = Function code supported
ILLEGAL_FUNCTION = Function code not supported
------------------------------------------------------------------------------*/
static uint8_t mb_check_illegal_function(uint8_t function_code)
{
   switch(function_code)
   {
   case READ_DISCRETE_INPUTS:             return ALL_OK; break;
   case READ_COILS:                       return ALL_OK; break;
   case WRITE_SINGLE_COIL:                return ALL_OK; break;
   case WRITE_MULTIPLE_COILS:             return ALL_OK; break;
   case READ_INPUT_REGISTER:              return ALL_OK; break;
   case READ_HOLDING_REGISTERS:           return ALL_OK; break;
   case WRITE_SINGLE_REGISTER:            return ALL_OK; break;
   case WRITE_MULTIPLE_REGISTERS:         return ALL_OK; break;
   case READ_WRITE_MULTIPLE_REGISTERS:    return ALL_OK; break;
   case MASK_WRITE_REGISTER:              return ALL_OK; break;
   case READ_FIFO_QUEUE:                  return ALL_OK; break;
   case READ_FILE_RECORD:                 return ALL_OK; break;
   case WRITE_FILE_RECORD:                return ALL_OK; break;
   case READ_EXCEPTION_STATUS:            return ALL_OK; break;
   case DIAGNOSTIC:                       return ALL_OK; break;
   case GET_COM_EVENT_COUNTER:            return ALL_OK; break;
   case GET_COM_EVENT_LOG:                return ALL_OK; break;
   case REPORT_SLAVE_ID:                  return ALL_OK; break;
   case READ_DEVICE_IDENTIFICATION:       return ALL_OK; break;
   case UMAS_FILE_TRANSFER:               return ALL_OK; break;
   default:                               return ILLEGAL_FUNCTION; break;
   } // switch(fc)
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
Return value:
ALL_OK = all OK
ILLEGAL_DATA_ADDRESS = Wrong data address
ILLEGAL_DATA_VALUE = Wrong data value
------------------------------------------------------------------------------*/
static uint8_t mb_check_illegal_data_address_and_value(uint16_t address,
                                                       uint16_t size)
{
   if((address < NUM_MIN_ADDRESS) || (address > NUM_MAX_ADDRESS))
   {
      return ILLEGAL_DATA_ADDRESS;
   }

   if((size < NUM_MIN_REGISTERS) || (size > NUM_MAX_REGISTERS))
   {
      return ILLEGAL_DATA_VALUE;
   }

   if((address + size) > NUM_MAX_ADDRESS)
   {
      return ILLEGAL_DATA_ADDRESS;
   }

   return ALL_OK;
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
Will return appropriate error values
------------------------------------------------------------------------------*/
static void mbtcp_send_adu(chanend c_xtcp,
                           xtcp_connection_t *conn,
                           chanend c_user)
{
   uint16_t length;
   s_modbus_tcp_adu_t adu;

   xmodbus_transmit(c_user, &adu);
   length = adu.length_field + SIZE_MODBUS_MBAP;

   xtcp_send(c_xtcp, &adu, length);
   xtcp_complete_send(c_xtcp);
   xtcp_close(c_xtcp, conn);
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
------------------------------------------------------------------------------*/
static void mbtcp_init_state(chanend c_xtcp, xtcp_connection_t *conn)
{
   xtcp_set_connection_appstate(c_xtcp, conn, 1u);
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
Will return appropriate error values
------------------------------------------------------------------------------*/
uint8_t mbtcp_handle_event(chanend c_xtcp,
                                  xtcp_connection_t *conn,
                                  chanend c_user)
{
   switch(conn->event)
   {
   case XTCP_IFUP:
   case XTCP_IFDOWN:
   case XTCP_ALREADY_HANDLED: return;
   default: break;
   }

   // Check if the connection is a http connection
   if (conn->local_port == PORT_LISTEN)
   {
      switch (conn->event)
      {
      case XTCP_NEW_CONNECTION:
         {
            mbtcp_init_state(c_xtcp, conn);
            break;
         }
      case XTCP_RECV_DATA:
         {
            mbtcp_receive_adu(c_xtcp, conn, c_user);
            break;
         }
      case XTCP_SENT_DATA:
      case XTCP_REQUEST_DATA:
      case XTCP_RESEND_DATA:
         {
            mbtcp_send_adu(c_xtcp, conn, c_user);
            break;
         }
      case XTCP_TIMED_OUT:
      case XTCP_ABORTED:
      case XTCP_CLOSED:
      default: break;
      }
      conn->event = XTCP_ALREADY_HANDLED;
   }
   return 0u;
}

/*============================================================================*/
