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
Copyright (c) 2011, XMOS Ltd, All rights reserved
This software is freely distributable under a derivative of the University of 
Illinois/NCSA Open Source License posted in LICENSE.txt and at 
<http://github.xcore.com/>
==============================================================================*/

/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include <string.h>
#include <print.h>
#include "system.h"
#include "xtcp_client.h"
#include "modbus_tcp_server.h"
#include "vin_led.h"

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
static uint8_t adu[SIZE_MODBUS_ADU];
static uint8_t connection_state[NUM_MODBUS_CONNECTIONS];

/*------------------------------------------------------------------------------
Prototypes
------------------------------------------------------------------------------*/
static void mbtcp_send(chanend tcp_svr);
static void mbtcp_new_connection(chanend tcp_svr, xtcp_connection_t *conn);
static void mbtcp_recv(chanend tcp_svr, xtcp_connection_t *conn);
static uint8_t mbtcp_check_illegal_data_address_and_value(uint16_t address,
                                                          uint16_t size);
static uint8_t mbtcp_check_illegal_function(uint8_t function_code);
static uint8_t mbtcp_check_mbap();
static uint8_t mbtcp_parse_adu();
static void mbtcp_set_exception(uint8_t check_adu);

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
static uint8_t mbtcp_parse_adu()
{
   uint8_t check_mbap;
   uint8_t check_illegal_function;
   //uint8_t check_illegal_data_address_and_value;

   uint8_t function_code;
   //uint16_t address;
   //uint16_t size;

   check_mbap = mbtcp_check_mbap();

   if(check_mbap == ALL_OK)
   {
      // MBAP OK
      function_code = adu[INDEX_FUNCTION_CODE];
      check_illegal_function = mbtcp_check_illegal_function(function_code);

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
static uint8_t mbtcp_check_mbap()
{
   //uint16_t tid = (uint16_t)((uint16_t)(adu[INDEX_HI_TRANSACTION_ID] << 8u) +
   //                          (uint16_t)(adu[INDEX_HI_TRANSACTION_ID + 1u]));

   uint16_t pid = (uint16_t)((uint16_t)(adu[INDEX_PROTOCOL_ID] << 8u) +
                             (uint16_t)(adu[INDEX_PROTOCOL_ID + 1u]));

   uint16_t len = (uint16_t)((uint16_t)(adu[INDEX_LENGTH_FIELD] << 8u) +
                             (uint16_t)(adu[INDEX_LENGTH_FIELD + 1u]));

   if(1) // tid == mbtcp_get_transid())
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
static uint8_t mbtcp_check_illegal_function(uint8_t function_code)
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
static uint8_t mbtcp_check_illegal_data_address_and_value(uint16_t address,
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
Name: mbtcp_recv
Purpose:
Input:
Output:
Notes: Receive and parse data from client
------------------------------------------------------------------------------*/
static void mbtcp_recv(chanend tcp_svr, xtcp_connection_t *conn)
{
   uint16_t length;
   uint8_t check_adu;

   // Receive the data from the TCP stack
   length = xtcp_recv(tcp_svr, adu);
   check_adu = mbtcp_parse_adu();

   if(check_adu == ALL_OK)
   {
      // temporary
      vin_led(adu);
      adu[INDEX_LENGTH_FIELD] = 0u;
      adu[INDEX_LENGTH_FIELD + 1u] = 2u;
   }
   else
   {
      // send level 1 exception
      mbtcp_set_exception(check_adu);
   }

   xtcp_init_send(tcp_svr, conn);
}

/*------------------------------------------------------------------------------
Name: mbtcp_set_exception
Purpose:
Input:
Output:
Notes: Setup Exception code and response length
------------------------------------------------------------------------------*/
static void mbtcp_set_exception(uint8_t check_adu)
{
   adu[INDEX_FUNCTION_CODE] += 0x80u;
   adu[INDEX_LENGTH_FIELD] = 0u;
   adu[INDEX_LENGTH_FIELD + 1u] = 2u;
}

/*------------------------------------------------------------------------------
Name: mbtcp_send
Purpose:
Input:
Output:
Notes: Send Modbus TCP data
------------------------------------------------------------------------------*/
static void mbtcp_send(chanend tcp_svr)
{
   xtcp_send(tcp_svr, adu, 8u);
   // Terminates the send process
   xtcp_complete_send(tcp_svr);
   // Close the connection
   //xtcp_close(tcp_svr, conn);
}

/*------------------------------------------------------------------------------
Name: mbtcp_new_connection
Purpose:
Input:
Output:
Notes: Setup a new connection
------------------------------------------------------------------------------*/
static void mbtcp_new_connection(chanend tcp_svr, xtcp_connection_t *conn)
{
   uint8_t i;

   // Try and find an empty connection slot
   for (i = 0u; i < NUM_MODBUS_CONNECTIONS; i++)
   {
      if (!connection_state[i])
      {
         // found a free connection, exit this for loop
         break;
      }
   }

   // If no free connection slots were found, abort the connection
   if ( i == NUM_MODBUS_CONNECTIONS )
   {
      xtcp_abort(tcp_svr, conn);
   }
   // Otherwise, assign the connection to a slot
   else
   {
      connection_state[i] = 1u;
      xtcp_set_connection_appstate(tcp_svr,
                                   conn,
                                   (xtcp_appstate_t) &connection_state[i]);
   }
}

/*------------------------------------------------------------------------------
Name: modbus_tcp_send
Purpose:
Input:
Output:
Notes: Send Modbus TCP data
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Name: modbus_tcp_reset
Purpose:
Input:
Output:
Notes: Reset modbus frame and connections
------------------------------------------------------------------------------*/
void modbus_tcp_reset(chanend tcp_svr)
{
   uint16_t index;

   // flush modbus frame
   for(index = 0u; index < SIZE_MODBUS_ADU; index++)
   {
      adu[index] = 0u;
   }

   // flush all connections
   for(index = 0u; index < NUM_MODBUS_CONNECTIONS; index++)
   {
      connection_state[index] = 0u;
   }
   
   xtcp_listen(tcp_svr, LISTEN_PORT, XTCP_PROTOCOL_TCP);
}

/*------------------------------------------------------------------------------
Name: modbus_tcp_handle_event
Purpose:
Input:
Output:
Notes: Handle event received form TCP stack
------------------------------------------------------------------------------*/
void modbus_tcp_handle_event(chanend tcp_svr, xtcp_connection_t *conn)
{
   // Ignore already handled events
   switch (conn->event)
   {
   case XTCP_IFUP:
   case XTCP_IFDOWN:
   case XTCP_ALREADY_HANDLED:
      return;
   default:
      break;
   }

   // Check if the connection is a http connection
   //if (conn->local_port == 80) {
   if (conn->local_port == LISTEN_PORT) {
      switch (conn->event)
      {
      case XTCP_NEW_CONNECTION:
         mbtcp_new_connection(tcp_svr, conn);
         break;
      case XTCP_RECV_DATA:
         mbtcp_recv(tcp_svr, conn);
         break;
      case XTCP_SENT_DATA:
      case XTCP_REQUEST_DATA:
      case XTCP_RESEND_DATA:
         mbtcp_send(tcp_svr);
         break;
      case XTCP_TIMED_OUT:
      case XTCP_ABORTED:
      case XTCP_CLOSED:
         modbus_tcp_reset(tcp_svr);
         break;
      default: break;
      }
      conn->event = XTCP_ALREADY_HANDLED;
   }
   return;
}

/*============================================================================*/
