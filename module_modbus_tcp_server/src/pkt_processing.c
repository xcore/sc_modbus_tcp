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

/*---------------------------------------------------------------------------
 global variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 static variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 prototypes
 ---------------------------------------------------------------------------*/

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

    uint16_t d1 = (uint16_t)((uint16_t)(data[INDEX_START_DATA] << 8u) +
                             (uint16_t)(data[INDEX_START_DATA + 1u]));

    uint16_t d2 = (uint16_t)((uint16_t)(data[INDEX_START_DATA + 2u] << 8u) +
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
            exception_code = check_1bit_read_range(d1, d2);

            if(!exception_code)
            {
                // No exceptions till now; we can continue with memory access
                uint16_t i;
                for(i = starting_address; i <= end_address; i++)
                {
                    // read or write coils
                    // any error here should be exception code 4u
                }
            }
            break;
        }
    #endif

    #ifdef READ_DISCRETE_INPUTS
        case READ_DISCRETE_INPUTS:
        {
            exception_code = check_1bit_read_range(d1, d2);

            if(!exception_code)
            {
                // No exceptions till now; we can continue with memory access
                uint16_t i;
                for(i = starting_address; i <= end_address; i++)
                {
                    // read or write coils
                    // any error here should be exception code 4u
                }
            }
            break;
        }
    #endif

    #ifdef READ_HOLDING_REGISTERS
        case READ_HOLDING_REGISTERS:
        {
            exception_code = check_register_read_range(d1, d2);

            if(!exception_code)
            {
                // No exceptions till now; we can continue with memory access
                uint16_t i;
                for(i = starting_address; i <= end_address; i++)
                {
                    // read or write coils
                    // any error here should be exception code 4u
                }
            }
            break;
        }
    #endif

    #ifdef READ_INPUT_REGISTER
        case READ_INPUT_REGISTER:
        {
            exception_code = check_register_read_range(d1, d2);

            if(!exception_code)
            {
                // No exceptions till now; we can continue with memory access
                uint16_t i;
                for(i = starting_address; i <= end_address; i++)
                {
                    // read or write coils
                    // any error here should be exception code 4u
                }
            }
            break;
        }
    #endif

    #ifdef WRITE_SINGLE_COIL
        case WRITE_SINGLE_COIL:
        {
            exception_code = check_read_range(d1, d2, 0x0001, 0x007D,
                                              0x0000u, 0xFFFFu);

            if(!exception_code)
            {
                // No exceptions till now; we can continue with memory access
                uint16_t i;
                for(i = starting_address; i <= end_address; i++)
                {
                    // read or write coils
                    // any error here should be exception code 4u
                }
            }
            break;
        }
    #endif

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
        data[INDEX_LENGTH_FIELD] = 3u;

        // Set function code + 0x80
        data[INDEX_FUNCTION_CODE] = function_code + 0x80;

        // Set exception code as illegal function
        data[INDEX_START_DATA] = exception_code;

        // Set response length
        response_length = SIZE_MODBUS_MBAP + 1u;
    }
    else
    {

    }
    return response_length;
}

/** =========================================================================
 *  check_read_range
 *
 *  \param start_address  start address
 *  \param quantity       quantity to read
 *
 **/
uint8_t check_1bit_read_range(uint16_t start_address, uint16_t quantity)
{
    uint8_t exception_code = 0u;

    if(quantity <= 0x0001 || quantity >= 0x07D0)
    {
        exception_code = 3u;
    }
    else
    {
        uint16_t end_address = start_address + quantity;

        if(!((starting_address >= 0x0000) && (end_address <= 0xFFFF)))
        {
            exception_code = 2u;
        }
    }
    return exception_code;
}

/** =========================================================================
 *  check_register_read_range
 *
 *  \param start_address  start address
 *  \param quantity       quantity to read
 *
 **/
uint8_t check_register_read_range(uint16_t start_address, uint16_t quantity)
{
    uint8_t exception_code = 0u;

    if(quantity <= 0x0001 || quantity >= 0x007D)
    {
        exception_code = 3u;
    }
    else
    {
        uint16_t end_address = start_address + quantity;

        if(!((starting_address >= 0x0000) && (end_address <= 0xFFFF)))
        {
            exception_code = 2u;
        }
    }
    return exception_code;
}

/*=========================================================================*/
