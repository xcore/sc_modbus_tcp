// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 1. ``modbus_tcp_parse_request()`` is called by the TCP layer.
 2. The data received is parsed for Modbus command.
 3. Appropriate call-backs to the user application is made based on the command
 received.
 4. After getting the data from application, a Modbus TCP response replaces the
 contents of ``data`` (the TCP data).
 5. ``modbus_tcp_parse_request()`` exits. The TCP layer can now send ``data``.

 ===========================================================================*/

/*---------------------------------------------------------------------------
 include files
 ---------------------------------------------------------------------------*/
#include "mbtcp_parser.h"
#include "mb_codes.h"
#include "mb_util.h"

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/
// Default quantity limits
#define MODBUS_ADDRESS_START        0x0000
#define MODBUS_ADDRESS_END          0xFFFF
#define MODBUS_QUANTITY_START       0x0001
#define MODBUS_QUANTITY_1BIT_END    0x07D0
#define MODBUS_QUANTITY_16BIT_END   0x007D

// Modbus protocol ID and data sizes
#define MODBUS_PROTOCOL_IDENTIFIER  0x0000u
#define MODBUS_SIZE_MBAP            7u

// Big Endian format in Modbus data
#define MODBUS_INDEX_PROTOCOL_ID    2u
#define MODBUS_INDEX_LENGTH_FIELD   4u
#define MODBUS_INDEX_FUNCTION_CODE  7u
#define MODBUS_INDEX_START_DATA     8u

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

enum modbus_exception
{
  ILLEGAL_FUNCTION     = 0x01,
  ILLEGAL_DATA_ADDRESS = 0x02,
  ILLEGAL_DATA_VALUE   = 0x03,
  SLAVE_DEVICE_FAILURE = 0x04,
};

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
 *  \param qty_hi     qty hi limit
 *  \param qty_lo     qty lo limit
 *  \param data       data received from tcp
 *  \return int       modbus response length
 *
 **/
static int modbus_read_data(chanend c_modbus,
                            uint8_t fn_code,
                            uint16_t qty,
                            uint16_t address,
                            uint16_t qty_hi,
                            uint16_t qty_lo,
                            char *data);

/*---------------------------------------------------------------------------
 check range
 ---------------------------------------------------------------------------*/
static uint8_t check_range(int value, uint16_t limit_lo, uint16_t limit_hi)
{
  return (!((value < limit_lo) || (value > limit_hi)));
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
static int modbus_read_data(chanend c_modbus,
                            uint8_t fn_code,
                            uint16_t qty,
                            uint16_t address,
                            uint16_t qty_hi,
                            uint16_t qty_lo,
                            char *data)
{
  int response_length;

  // Modbus Application Protocol V1 1B - Section 6.1
  // Check if quantity is within range
  if(check_range(qty, qty_lo, qty_hi))
  {
    uint16_t end_qty_address = address + qty;

    // Check End address
    if(check_range(end_qty_address, MODBUS_ADDRESS_START, MODBUS_ADDRESS_END))
    {
      uint16_t i;
      uint16_t index_status = MODBUS_SIZE_MBAP + 2u;
      uint8_t index_byte, index_bit;
      uint16_t read_value = 0;

      // Get number of bytes = qty/8 (+1)**
      uint8_t byte_count = modbus_get_byte_count(qty);

      switch(fn_code)
      {
        case MODBUS_READ_COIL:
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
            read_value = access_external_device(c_modbus,
                                                MODBUS_READ_COIL,
                                                (i+address),
                                                0);
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

        case MODBUS_READ_DISCRETE_INPUT:
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
            read_value = access_external_device(c_modbus,
                                                MODBUS_READ_DISCRETE_INPUT,
                                                (i+address),
                                                0);

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

        case MODBUS_READ_HOLDING_REGISTER:
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
            read_value = access_external_device(c_modbus,
                                                MODBUS_READ_HOLDING_REGISTER,
                                                (i+address),
                                                0);

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

        case MODBUS_READ_INPUT_REGISTER:
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
            read_value = access_external_device(c_modbus,
                                                MODBUS_READ_INPUT_REGISTER,
                                                (i+address),
                                                0);

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
int modbus_tcp_parse_request(chanend c_modbus, char *data, int len)
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
    case MODBUS_READ_COIL:
    {
      response_length = modbus_read_data(c_modbus,
                                         function_code,
                                         qty,
                                         address,
                                         MODBUS_QUANTITY_1BIT_END,
                                         MODBUS_QUANTITY_START,
                                         data);
      break;
    }

    case MODBUS_READ_DISCRETE_INPUT:
    {
      response_length = modbus_read_data(c_modbus,
                                         function_code,
                                         qty,
                                         address,
                                         MODBUS_QUANTITY_1BIT_END,
                                         MODBUS_QUANTITY_START,
                                         data);
      break;
    }

    case MODBUS_READ_HOLDING_REGISTER:
    {
      response_length = modbus_read_data(c_modbus,
                                         function_code,
                                         qty,
                                         address,
                                         MODBUS_QUANTITY_16BIT_END,
                                         MODBUS_QUANTITY_START,
                                         data);
      break;
    }

    case MODBUS_READ_INPUT_REGISTER:
    {
      response_length = modbus_read_data(c_modbus,
                                         function_code,
                                         qty,
                                         address,
                                         MODBUS_QUANTITY_16BIT_END,
                                         MODBUS_QUANTITY_START,
                                         data);
      break;
    }

    case MODBUS_WRITE_SINGLE_COIL:
    {
      // Modbus Application Protocol V1 1B - Section 6.1
      // Check if quantity is within range
      if (qty == 0x0000 || qty == 0xFF00)
      {
        uint16_t index_status = MODBUS_SIZE_MBAP + 1u;
        uint16_t read_value = 0;

        // Get number of bytes
        uint8_t byte_count = 4u;

        // Update response length
        response_length = 1u + byte_count;

        // Unit identifier & Function code remains the same
        // Update data with Length field
        data[MODBUS_INDEX_LENGTH_FIELD]
            = (uint8_t) ((uint16_t) (response_length + 1u) >> 8u);
        data[MODBUS_INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

        // any error here should be exception code 4u
        read_value = access_external_device(c_modbus,
                                            MODBUS_WRITE_SINGLE_COIL,
                                            address,
                                            qty);

        if (read_value)
        {
        }
        else
        {
          modbus_exception_code = SLAVE_DEVICE_FAILURE;
        }
      } // if qty range check

      else
      {
        modbus_exception_code = ILLEGAL_DATA_VALUE;
      }
      break;
    } // case WRITE_SINGLE_COIL

    case MODBUS_WRITE_SINGLE_REGISTER:
    {
      // Modbus Application Protocol V1 1B - Section 6.1
      // Check if quantity is within range
      if (check_range(qty, MODBUS_QUANTITY_START, MODBUS_QUANTITY_16BIT_END))
      {
        uint16_t index_status = MODBUS_SIZE_MBAP + 1u;
        uint16_t read_value = 0;

        // Get number of bytes
        uint8_t byte_count = 4u;

        // Update response length
        response_length = 1u + byte_count;

        // Unit identifier & Function code remains the same
        // Update data with Length field
        data[MODBUS_INDEX_LENGTH_FIELD]
            = (uint8_t) ((uint16_t) (response_length + 1u) >> 8u);
        data[MODBUS_INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

        // any error here should be exception code 4u
        read_value = access_external_device(c_modbus,
                                            MODBUS_WRITE_SINGLE_REGISTER,
                                            address,
                                            qty);

        if (read_value)
        {
        }
        else
        {
          modbus_exception_code = SLAVE_DEVICE_FAILURE;
        }

      } // if qty range check

      else
      {
        modbus_exception_code = ILLEGAL_DATA_VALUE;
      }
      break;
    } // case WRITE_SINGLE_REGISTER

    default:
    {
      modbus_exception_code = ILLEGAL_FUNCTION;
      break;
    } // default
  } // switch(function_code)


  if (modbus_exception_code)
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
