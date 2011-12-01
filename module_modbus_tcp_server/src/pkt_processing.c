// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Filename:
 Project :
 Author  :
 Version :
 Purpose
 -----------------------------------------------------------------------------


 ===========================================================================*/

/*---------------------------------------------------------------------------
 include files
 ---------------------------------------------------------------------------*/
#include "pkt_processing.h"
#include <print.h>
/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/
#define ALL_OK                                  0x00u

// Big Endian format in Modbus data
#define INDEX_TRANSACTION_ID                    0u
#define INDEX_PROTOCOL_ID                       2u
#define INDEX_LENGTH_FIELD                      4u
#define INDEX_UNIT_ID                           6u
#define INDEX_FUNCTION_CODE                     7u
#define INDEX_START_DATA                        8u

/*---------------------------------------------------------------------------
 extern variables
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

/*---------------------------------------------------------------------------
 prototypes
 ---------------------------------------------------------------------------*/
uint8_t check_range(uint16_t value, uint16_t limit_lo, uint16_t limit_hi);
uint8_t get_byte_count(uint16_t qty);

void updata_data(char *data);
/*---------------------------------------------------------------------------
 implementation
 ---------------------------------------------------------------------------*/

/** =========================================================================
 *  modbus_tcp_process_frame
 *  Hard coded values as defined in Modbus Application Protocol V1_1b
 *
 *  \param data      pointer to data from TCP
 *  \param length    length of TCP data received
 *
 **/
int modbus_tcp_process_frame(char *data, int length)
{
    int response_length;

    uint8_t exception_code = 0u; // No error

    uint16_t pid = (uint16_t)((uint16_t)(data[INDEX_PROTOCOL_ID] << 8u) +
                              (uint16_t)(data[INDEX_PROTOCOL_ID + 1u]));

    uint8_t function_code = data[INDEX_FUNCTION_CODE];

    uint16_t address = (uint16_t)((uint16_t)(data[INDEX_START_DATA] << 8u) +
                                  (uint16_t)(data[INDEX_START_DATA + 1u]));

    uint16_t qty = (uint16_t)((uint16_t)(data[INDEX_START_DATA + 2u] << 8u) +
                              (uint16_t)(data[INDEX_START_DATA + 3u]));

    // Check protocol ID
    if(pid != MODBUS_PROTOCOL_IDENTIFIER)
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
            // Modbus Application Protocol V1 1B - Section 6.1
            // Check if quantity is within range
            if(check_range(qty, QUANTITY_COIL_START, QUANTITY_COIL_END))
            {
                uint16_t end_qty_address = address + qty;

                // Check start and end address
                if(check_range(address, ADDRESS_COIL_START, ADDRESS_COIL_END) &&
                   check_range(end_qty_address, ADDRESS_COIL_START, ADDRESS_COIL_END))
                {
                    uint16_t i;
                    uint16_t index_status = SIZE_MODBUS_MBAP + 2u;
                    uint8_t index_byte, index_bit;

                    // Get number of bytes = qty/8 (+1)**
                    uint8_t byte_count = get_byte_count(qty);

                    // Update response length
                    response_length = 2u + byte_count;

                    // Update data with Length field
                    data[INDEX_LENGTH_FIELD] = (uint8_t)((uint16_t)(response_length + 1u) >> 8u);
                    data[INDEX_LENGTH_FIELD + 1u] = response_length + 1u;

                    // Unit identifier & Function code remains the same
                    // Update number of bytes to read
                    data[index_status - 1u] = byte_count;

                    // Reset response data before reading
                    for(i = 0u; i < byte_count; i++)
                    {
                        data[index_status + i] = 0u;
                    }
                    // Read
                    for(i = 0; i <= qty; i++)
                    {
                        index_byte = (i + 7u) / 8u;

                        if(i > 7u)
                        {
                            index_bit = i % 8u;
                        }
                        else
                        {
                            index_bit = i;
                        }


                        // any error here should be exception code 4u
                        //if(read_coil(i + address) == 1u)
                        {
                            data[index_status + index_byte] |= (1u << index_bit);
                            printintln(data[index_status + index_byte]);
                        }
                    } // for qty
                } // if address range check
                else
                {
                    exception_code = 2u;
                }
            }
            else
            {
                exception_code = 3u;
            }
            break;
        }
    #endif // READ_COILS
/*
    #ifdef READ_DISCRETE_INPUTS
        case READ_DISCRETE_INPUTS:
        {
            break;
        }
    #endif // READ_DISCRETE_INPUTS

    #ifdef READ_HOLDING_REGISTERS
        case READ_HOLDING_REGISTERS:
        {
            break;
        }
    #endif // READ_HOLDING_REGISTERS

    #ifdef READ_INPUT_REGISTER
        case READ_INPUT_REGISTER:
        {
            break;
        }
    #endif // READ_INPUT_REGISTER

    #ifdef WRITE_SINGLE_COIL
        case WRITE_SINGLE_COIL:
        {
            break;
        }
    #endif // WRITE_SINGLE_COIL
*/
        default:
        {
            // Any other function code is either not defined or a user defined
            #ifdef USER_DEFINED_FUNCTION_CODE_HOOK
                user_defined_processing_hook();
            #else
                exception_code = 1u;
            #endif
            break;
        } // default
    } // switch(function_code)



    if(exception_code)
    {
        // Build exception response

        // Set length field
        data[INDEX_LENGTH_FIELD] = 0u;
        data[INDEX_LENGTH_FIELD + 1u] = 3u;

        // Set function code + 0x80
        data[INDEX_FUNCTION_CODE] = function_code + 0x80;

        // Set exception code as illegal function
        data[INDEX_START_DATA] = exception_code;

        // Set response length
        response_length = 2u;
    }
    else
    {

    }
    return (SIZE_MODBUS_MBAP + response_length);
}

/** =========================================================================
 *  check_range
 *
 *  \param value      value to check
 *  \param limit_lo   bottom limit
 *  \param limit_hi   top limit
 *
 **/
uint8_t check_range(uint16_t value, uint16_t limit_lo, uint16_t limit_hi)
{
    uint8_t value_within_range = 0u; // false

    if(value < limit_lo || value > limit_hi)
    {}
    else
    {
        value_within_range = 1u;
    }
    return value_within_range;
}

/** =========================================================================
 *  get_byte_count
 *
 *  \param value      value to check
 *  \param limit_lo   bottom limit
 *  \param limit_hi   top limit
 *
 **/
uint8_t get_byte_count(uint16_t qty)
{
    uint8_t rtnval = (uint8_t)((qty + 7u) / 8u);
    return rtnval;
}

void updata_data(char *data)
{
    data[0] = 1u;
}
/*=========================================================================*/
