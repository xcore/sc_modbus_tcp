// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 1. ``modbus_tcp_parse_request()`` is called by the user application after
 receiving data from the TCP layer.
 2. The data received is parsed for Modbus command.
 3. Appropriate call-backs to the user application is made based on the command
 received.
 4. After getting the data from call-back functions, a Modbus TCP response
 replaces the contents of ``data`` (the TCP data).
 5. ``modbus_tcp_parse_request()`` exits. The user application must now send
 this response data over TCP to the host.
 
 ===========================================================================*/

/*---------------------------------------------------------------------------
 include files
 ---------------------------------------------------------------------------*/
#include "modbus_tcp.h"

/*
 * Include this file for call-back functions
 */
#ifdef __modbus_conf_h_exists__
#include "modbus_conf.h"
#endif

#include <print.h>

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/

// Default addresses and quantity limits

/*
 * Start address for coils
 */
#ifndef MODBUS_ADDRESS_COIL_START
    #define MODBUS_ADDRESS_COIL_START                      0x0000
#endif
/*
 * End address for coils
 */
#ifndef MODBUS_ADDRESS_COIL_END
    #define MODBUS_ADDRESS_COIL_END                        0xFFFF
#endif
/*
 * Start address for Holding Register
 */
#ifndef MODBUS_ADDRESS_HOLDING_REGISTER_START
    #define MODBUS_ADDRESS_HOLDING_REGISTER_START          0x0000
#endif
/*
 * End address for Holding Register
 */
#ifndef MODBUS_ADDRESS_HOLDING_REGISTER_END
    #define MODBUS_ADDRESS_HOLDING_REGISTER_END            0xFFFF
#endif
/*
 * Start address for Input Register
 */
#ifndef MODBUS_ADDRESS_INPUT_REGISTER_START
    #define MODBUS_ADDRESS_INPUT_REGISTER_START            0x0000
#endif
/*
 * End address for Input Register
 */
#ifndef MODBUS_ADDRESS_INPUT_REGISTER_END
    #define MODBUS_ADDRESS_INPUT_REGISTER_END              0xFFFF
#endif
/*
 * Start address for Discrete Input
 */
#ifndef MODBUS_ADDRESS_DISCRETE_INPUT_START
    #define MODBUS_ADDRESS_DISCRETE_INPUT_START            0x0000
#endif
/*
 * End address for Discrete Input
 */
#ifndef MODBUS_ADDRESS_DISCRETE_INPUT_END
    #define MODBUS_ADDRESS_DISCRETE_INPUT_END              0xFFFF
#endif
/*
 * Start quantity for coils
 */
#ifndef MODBUS_QUANTITY_COIL_START
    #define MODBUS_QUANTITY_COIL_START                     0x0001
#endif
/*
 * End quantity for coils
 */
#ifndef MODBUS_QUANTITY_COIL_END
    #define MODBUS_QUANTITY_COIL_END                       0x07D0
#endif
/*
 * Start quantity for Holding Register
 */
#ifndef MODBUS_QUANTITY_HOLDING_REGISTER_START
    #define MODBUS_QUANTITY_HOLDING_REGISTER_START         0x0001
#endif
/*
 * End quantity for Holding Register
 */
#ifndef MODBUS_QUANTITY_HOLDING_REGISTER_END
    #define MODBUS_QUANTITY_HOLDING_REGISTER_END           0x007D
#endif
/*
 * Start quantity for Input Register
 */
#ifndef MODBUS_QUANTITY_INPUT_REGISTER_START
    #define MODBUS_QUANTITY_INPUT_REGISTER_START           0x0001
#endif
/*
 * End quantity for Input Register
 */
#ifndef MODBUS_QUANTITY_INPUT_REGISTER_END
    #define MODBUS_QUANTITY_INPUT_REGISTER_END             0x007D
#endif
/*
 * Start quantity for Discrete Inputs
 */
#ifndef MODBUS_QUANTITY_DISCRETE_INPUT_START
    #define MODBUS_QUANTITY_DISCRETE_INPUT_START           0x0001
#endif
/*
 * End quantity for Discrete Inputs
 */
#ifndef MODBUS_QUANTITY_DISCRETE_INPUT_END
    #define MODBUS_QUANTITY_DISCRETE_INPUT_END             0x07D0
#endif
/*
 * Write quantity for coils 1
 */
#ifndef MODBUS_WRITE_QUANTITY_1
   #define MODBUS_WRITE_QUANTITY_1                         0x0000
#endif
/*
 * Write quantity for coils 2
 */
#ifndef MODBUS_WRITE_QUANTITY_2
   #define MODBUS_WRITE_QUANTITY_2                         0xFF00
#endif

/*
 * Currently supported Modbus TCP commands
 */
#define READ_COILS                              0x01
#define READ_DISCRETE_INPUTS                    0x02
#define READ_HOLDING_REGISTERS                  0x03
#define READ_INPUT_REGISTER                     0x04
#define WRITE_SINGLE_COIL                       0x05
#define WRITE_SINGLE_REGISTER                   0x06

// Modbus protocol ID and data sizes
#define MODBUS_PROTOCOL_IDENTIFIER              0x0000u
#define MODBUS_SIZE_MBAP                        7u

// Big Endian format in Modbus data
#define MODBUS_INDEX_PROTOCOL_ID                2u
#define MODBUS_INDEX_LENGTH_FIELD               4u
#define MODBUS_INDEX_FUNCTION_CODE              7u
#define MODBUS_INDEX_START_DATA                 8u

/*---------------------------------------------------------------------------
 ports and clocks
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 typedefs
 ---------------------------------------------------------------------------*/
typedef unsigned char   uint8_t;
typedef signed char     int8_t;
typedef unsigned short  uint16_t;
typedef signed short    int16_t;
typedef unsigned int    uint32_t;
typedef signed int      int32_t;

/*---------------------------------------------------------------------------
 global variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 static variables
 ---------------------------------------------------------------------------*/
static uint8_t modbus_exception_code;

/*---------------------------------------------------------------------------
 static prototypes
 ---------------------------------------------------------------------------*/

/** =========================================================================
 *  check_range
 *
 *  \param value      value to check
 *  \param limit_lo   bottom limit
 *  \param limit_hi   top limit
 *  \return char      1 is ok; 0 is outside range
 *
 **/
static uint8_t check_range(int value, uint16_t limit_lo, uint16_t limit_hi);

/** =========================================================================
 *  get_byte_count
 *
 *  \param qty      quantity
 *  \return char    number of bytes required
 *
 **/
static uint8_t get_byte_count(uint16_t qty);

/** =========================================================================
 *  modbus_read_data
 *  Read data from user defined memory interface.
 *
 *  \param fn_code    function code
 *  \param qty        quantity
 *  \param address    address
 *  \param address_hi address hi limit
 *  \param address_lo address lo limit
 *  \param qty_hi     qty hi limit
 *  \param qty_lo     qty lo limit
 *  \param data       data received from tcp
 *  \return int       modbus response length
 *
 **/
static int modbus_read_data(uint8_t fn_code,
                            uint16_t qty,
                            uint16_t address,
                            uint16_t address_hi,
                            uint16_t address_lo,
                            uint16_t qty_hi,
                            uint16_t qty_lo,
                            char *data);

/*---------------------------------------------------------------------------
 check range
 ---------------------------------------------------------------------------*/
static uint8_t check_range(int value, uint16_t limit_lo, uint16_t limit_hi)
{
  return (!(value < limit_lo || value > limit_hi));
}

/*---------------------------------------------------------------------------
 get number of bytes required
 ---------------------------------------------------------------------------*/
static uint8_t modbus_get_byte_count(uint16_t qty)
{
  return (uint8_t) ((qty + 7u) / 8u);
}

/*---------------------------------------------------------------------------
 modbus read data
 ---------------------------------------------------------------------------*/
static int modbus_read_data(uint8_t fn_code,
                            uint16_t qty,
                            uint16_t address,
                            uint16_t address_hi,
                            uint16_t address_lo,
                            uint16_t qty_hi,
                            uint16_t qty_lo,
                            char *data)
{
  int response_length;

  // Modbus Application Protocol V1 1B - Section 6.1
  // Check if quantity is within range
  if (check_range(qty, qty_lo, qty_hi))
  {
    uint16_t end_qty_address = address + qty;

    // Check start and end address
    if (check_range(address, address_lo, address_hi)
        && check_range(end_qty_address, address_lo, address_hi))
    {
      uint16_t i;
      uint16_t index_status = MODBUS_SIZE_MBAP + 2u;
      uint8_t index_byte, index_bit;
      uint16_t read_value = 0;

      // Get number of bytes = qty/8 (+1)**
      uint8_t byte_count = modbus_get_byte_count(qty);

      switch(fn_code)
      {
#ifdef READ_COILS
        case READ_COILS:
        {
          // Update response length
          response_length = 2u + byte_count;

          // Update data with Length field
          data[MODBUS_INDEX_LENGTH_FIELD] = (uint8_t) ((uint16_t)
              (response_length + 1u) >> 8u);
          data[MODBUS_INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

          // Unit identifier & Function code remains the same
          // Update number of bytes to read
          data[index_status - 1u] = byte_count;

          // Reset response data before reading
          for (i = 0u; i < byte_count; i++)
          {
            data[index_status + i] = 0u;
          }
          // Read
          for (i = 0; i < qty; i++)
          {
            index_byte = i / 8u;
            index_bit = i % 8u;

            // any error here should be exception code 4u
#ifdef MB_READ_COILS
            read_value = (uint16_t)(MB_READ_COILS((i + address)));
#endif

            if (read_value == 1u)
            {
              data[index_status + index_byte] |= (1u << index_bit);
            }
            else if (read_value == 0u)
            {
              data[index_status + index_byte] &= ~(1u << index_bit);
            }
            else
            {
              modbus_exception_code = SLAVE_DEVICE_FAILURE;
              break;
            }
          } // for qty
          break;
        } // case READ_COILS
#endif // READ_COILS
#ifdef READ_DISCRETE_INPUTS
        case READ_DISCRETE_INPUTS:
        {
          // Update response length
          response_length = 2u + byte_count;

          // Update data with Length field
          data[MODBUS_INDEX_LENGTH_FIELD] = (uint8_t) ((uint16_t)
              (response_length + 1u) >> 8u);
          data[MODBUS_INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

          // Unit identifier & Function code remains the same
          // Update number of bytes to read
          data[index_status - 1u] = byte_count;

          // Reset response data before reading
          for (i = 0u; i < byte_count; i++)
          {
            data[index_status + i] = 0u;
          }
          // Read
          for (i = 0; i < qty; i++)
          {
            index_byte = i / 8u;
            index_bit = i % 8u;

            // any error here should be exception code 4u
#ifdef MB_READ_DISCRETE_INPUTS
            read_value = (uint16_t)(MB_READ_DISCRETE_INPUTS((i + address)));
#endif

            if (read_value == 1u)
            {
              data[index_status + index_byte] |= (1u << index_bit);
            }
            else if (read_value == 0u)
            {
              data[index_status + index_byte] &= ~(1u << index_bit);
            }
            else
            {
              modbus_exception_code = SLAVE_DEVICE_FAILURE;
              break;
            }
          } // for qty
          break;
        } // case READ_DISCRETE_INPUTS
#endif // READ_DISCRETE_INPUTS
#ifdef READ_HOLDING_REGISTERS
        case READ_HOLDING_REGISTERS:
        {
          byte_count = 2u * qty;

          // Update response length
          response_length = 2u + byte_count;

          // Update data with Length field
          data[MODBUS_INDEX_LENGTH_FIELD] = (uint8_t) ((uint16_t)
              (response_length + 1u) >> 8u);
          data[MODBUS_INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

          // Unit identifier & Function code remains the same
          // Update number of bytes to read
          data[index_status - 1u] = byte_count;

          // Reset response data before reading
          for (i = 0u; i < byte_count; i++)
          {
            data[index_status + i] = 0u;
          }
          // Read
          for (i = 0; i < qty; i++)
          {
            // any error here should be exception code 4u
#ifdef MB_READ_HOLDING_REGISTERS
            read_value = (uint16_t)(MB_READ_HOLDING_REGISTERS((i + address)));
#endif

            if (read_value)
            {
              data[index_status + (i * 2)] = (uint8_t) ((uint16_t)
                  (read_value) >> 8u);
              data[index_status + (i * 2) + 1u] = read_value;
            }
            else
            {
              modbus_exception_code = SLAVE_DEVICE_FAILURE;
              break;
            }
          } // for qty
          break;
        } // case READ_HOLDING_REGISTERS
#endif // READ_HOLDING_REGISTERS
#ifdef READ_INPUT_REGISTER
        case READ_INPUT_REGISTER:
        {
          byte_count = 2u * qty;

          // Update response length
          response_length = 2u + byte_count;

          // Update data with Length field
          data[MODBUS_INDEX_LENGTH_FIELD] = (uint8_t) ((uint16_t)
              (response_length + 1u) >> 8u);
          data[MODBUS_INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

          // Unit identifier & Function code remains the same
          // Update number of bytes to read
          data[index_status - 1u] = byte_count;

          // Reset response data before reading
          for (i = 0u; i < byte_count; i++)
          {
            data[index_status + i] = 0u;
          }
          // Read
          for (i = 0; i < qty; i++)
          {
            // any error here should be exception code 4u
#ifdef MB_READ_INPUT_REGISTERS
            read_value = (uint16_t)(MB_READ_INPUT_REGISTERS((i + address)));
#endif

            if (read_value)
            {
              data[index_status + (i * 2)] = (uint8_t) ((uint16_t)
                  (read_value) >> 8u);
              data[index_status + (i * 2) + 1u] = read_value;
            }
            else
            {
              modbus_exception_code = SLAVE_DEVICE_FAILURE;
              break;
            }
          } // for qty
          break;
        } // case READ_INPUT_REGISTER
#endif // READ_INPUT_REGISTER
      }
    } // if address range check

    else
    {
      modbus_exception_code = ILLEGAL_DATA_ADDRESS;
    }
  }
  else
  {
    modbus_exception_code = ILLEGAL_DATA_VALUE;
  }

  return response_length;
}

/*---------------------------------------------------------------------------
 parse modbus request
 ---------------------------------------------------------------------------*/
int modbus_tcp_parse_request(char *data, int len)
{
  int response_length;

  uint16_t pid = (uint16_t) ((uint16_t) (data[MODBUS_INDEX_PROTOCOL_ID] << 8u)
      + (uint16_t) (data[MODBUS_INDEX_PROTOCOL_ID + 1u]));

  uint8_t function_code = data[MODBUS_INDEX_FUNCTION_CODE];

  uint16_t address = (uint16_t) ((uint16_t) (data[MODBUS_INDEX_START_DATA]
      << 8u) + (uint16_t) (data[MODBUS_INDEX_START_DATA + 1u]));

  uint16_t qty = (uint16_t) ((uint16_t) (data[MODBUS_INDEX_START_DATA + 2u]
      << 8u) + (uint16_t) (data[MODBUS_INDEX_START_DATA + 3u]));

  modbus_exception_code = 0u;

  // Check protocol ID
  if (pid != MODBUS_PROTOCOL_IDENTIFIER)
  {
    response_length = 0u;
    return response_length;
  }

  // Check Function code support
  switch(function_code)
  {

#ifdef READ_COILS
    case READ_COILS:
    {
      response_length = modbus_read_data(
          function_code,
          qty,
          address,
          MODBUS_ADDRESS_COIL_END,
          MODBUS_ADDRESS_COIL_START,
          MODBUS_QUANTITY_COIL_END,
          MODBUS_QUANTITY_COIL_START,
          data);
      break;
    }
#endif // READ_COILS
#ifdef READ_DISCRETE_INPUTS
    case READ_DISCRETE_INPUTS:
    {
      response_length = modbus_read_data(
          function_code,
          qty,
          address,
          MODBUS_ADDRESS_DISCRETE_INPUT_END,
          MODBUS_ADDRESS_DISCRETE_INPUT_START,
          MODBUS_QUANTITY_DISCRETE_INPUT_END,
          MODBUS_QUANTITY_DISCRETE_INPUT_START,
          data);
      break;
    }
#endif // READ_DISCRETE_INPUTS
#ifdef READ_HOLDING_REGISTERS
    case READ_HOLDING_REGISTERS:
    {
      response_length = modbus_read_data(
          function_code,
          qty,
          address,
          MODBUS_ADDRESS_HOLDING_REGISTER_END,
          MODBUS_ADDRESS_HOLDING_REGISTER_START,
          MODBUS_QUANTITY_HOLDING_REGISTER_END,
          MODBUS_QUANTITY_HOLDING_REGISTER_START,
          data);
      break;
    }
#endif // READ_HOLDING_REGISTERS
#ifdef READ_INPUT_REGISTER
    case READ_INPUT_REGISTER:
    {
      response_length = modbus_read_data(
          function_code,
          qty,
          address,
          MODBUS_ADDRESS_INPUT_REGISTER_END,
          MODBUS_ADDRESS_INPUT_REGISTER_START,
          MODBUS_QUANTITY_INPUT_REGISTER_END,
          MODBUS_QUANTITY_INPUT_REGISTER_START,
          data);
      break;
    }
#endif // READ_INPUT_REGISTER
#ifdef WRITE_SINGLE_COIL
    case WRITE_SINGLE_COIL:
    {
      // Modbus Application Protocol V1 1B - Section 6.1
      // Check if quantity is within range
      if (qty == MODBUS_WRITE_QUANTITY_1 || qty == MODBUS_WRITE_QUANTITY_2)
      {
        // Check address range
        if (check_range(address,
                MODBUS_ADDRESS_COIL_START,
                MODBUS_ADDRESS_COIL_END))
        {
          uint16_t index_status = MODBUS_SIZE_MBAP + 1u;
          uint16_t read_value = 0;

          // Get number of bytes
          uint8_t byte_count = 4u;

          // Update response length
          response_length = 1u + byte_count;

          // Unit identifier & Function code remains the same
          // Update data with Length field
          data[MODBUS_INDEX_LENGTH_FIELD] = (uint8_t) ((uint16_t)
              (response_length + 1u) >> 8u);
          data[MODBUS_INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

          // any error here should be exception code 4u
#ifdef MB_WRITE_SINGLE_COIL
          read_value = (uint16_t)(MB_WRITE_SINGLE_COIL(address, qty));
#endif

          if (read_value)
          {}
          else
          {
            modbus_exception_code = SLAVE_DEVICE_FAILURE;
          }

        } // if address range check

        else
        {
          modbus_exception_code = ILLEGAL_DATA_ADDRESS;
        }
      } // if qty range check

      else
      {
        modbus_exception_code = ILLEGAL_DATA_VALUE;
      }
      break;
    } // case WRITE_SINGLE_COIL
#endif // WRITE_SINGLE_COIL
#ifdef WRITE_SINGLE_REGISTER
    case WRITE_SINGLE_REGISTER:
    {
      // Modbus Application Protocol V1 1B - Section 6.1
      // Check if quantity is within range
      if (check_range(qty,
              MODBUS_QUANTITY_HOLDING_REGISTER_START,
              MODBUS_QUANTITY_HOLDING_REGISTER_END))
      {
        // Check address range
        if (check_range(address,
                MODBUS_ADDRESS_HOLDING_REGISTER_START,
                MODBUS_ADDRESS_HOLDING_REGISTER_END))
        {
          uint16_t index_status = MODBUS_SIZE_MBAP + 1u;
          uint16_t read_value = 0;

          // Get number of bytes
          uint8_t byte_count = 4u;

          // Update response length
          response_length = 1u + byte_count;

          // Unit identifier & Function code remains the same
          // Update data with Length field
          data[MODBUS_INDEX_LENGTH_FIELD] = (uint8_t) ((uint16_t)
              (response_length + 1u) >> 8u);
          data[MODBUS_INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

          // any error here should be exception code 4u
#ifdef MB_WRITE_SINGLE_REGISTER
          read_value = (uint16_t)(MB_WRITE_SINGLE_REGISTER(address, qty));
#endif

          if (read_value)
          {}
          else
          {
            modbus_exception_code = SLAVE_DEVICE_FAILURE;
          }

        } // if address range check

        else
        {
          modbus_exception_code = ILLEGAL_DATA_ADDRESS;
        }
      } // if qty range check

      else
      {
        modbus_exception_code = ILLEGAL_DATA_VALUE;
      }
      break;
    } // case WRITE_SINGLE_REGISTER
#endif // WRITE_SINGLE_REGISTER
    default:
    {
      modbus_exception_code = ILLEGAL_FUNCTION;
      break;
    } // default
  } // switch(function_code)


  if(modbus_exception_code)
  {
    // Build exception response

    // Set length field
    data[MODBUS_INDEX_LENGTH_FIELD] = 0u;
    data[MODBUS_INDEX_LENGTH_FIELD + 1u] = 3u;

    // Set function code + 0x80
    data[MODBUS_INDEX_FUNCTION_CODE] = function_code + 0x80;

    // Set exception code as illegal function
    data[MODBUS_INDEX_START_DATA] = modbus_exception_code;

    // Set response length
    response_length = 2u;

    // Clear exception code
    modbus_exception_code = 0u;
  }
  else
  {}

  return (MODBUS_SIZE_MBAP + response_length);
}

/*==========================================================================*/
