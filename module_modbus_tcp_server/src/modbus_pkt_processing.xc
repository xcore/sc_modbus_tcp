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
#include "modbus_pkt_processing.h"
#include "application.h"

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/

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
static uint8_t modbus_exception_code;

/*---------------------------------------------------------------------------
 prototypes
 ---------------------------------------------------------------------------*/
static uint8_t modbus_check_range(int value, 
                                  uint16_t limit_lo, 
                                  uint16_t limit_hi);
static uint8_t modbus_get_byte_count(uint16_t qty);

static int modbus_read_data(uint8_t  fn_code,
                            uint16_t qty,
                            uint16_t address,
                            uint16_t address_hi,
                            uint16_t address_lo,
                            uint16_t qty_hi,
                            uint16_t qty_lo,
                            char data[]);

/*---------------------------------------------------------------------------
 implementation
 ---------------------------------------------------------------------------*/

/** =========================================================================
 *  check_range
 *
 *  \param value      value to check
 *  \param limit_lo   bottom limit
 *  \param limit_hi   top limit
 *  \return char      0 - if outside range and 1 - else
 *
 **/
static uint8_t modbus_check_range(int value, 
                                  uint16_t limit_lo, 
                                  uint16_t limit_hi)
{
    if(value < limit_lo || value > limit_hi)
    {
        return 0u;
    }
    else
    {
        return 1u;
    }
}

/** =========================================================================
 *  get_byte_count
 *
 *  \param qty      quantity
 *  \return char    number of bytes required
 *
 **/
static uint8_t modbus_get_byte_count(uint16_t qty)
{
    uint8_t rtnval = (uint8_t)((qty + 7u) / 8u);
    return rtnval;
}

/** =========================================================================
 *  modbus_process_frame
 *  Hard coded values as defined in Modbus Application Protocol V1_1b.
 *  Checks and processes modbus frame. The response is updated in data[].
 *
 *  \param data      pointer to data from TCP
 *  \param length    length of TCP data received
 *  \return int      response_length
 *
 **/
int modbus_process_frame(char data[], int length)
{
    int response_length;

    uint16_t pid = (uint16_t)((uint16_t)(data[MODBUS_INDEX_PROTOCOL_ID] << 8u) +
                              (uint16_t)(data[MODBUS_INDEX_PROTOCOL_ID + 1u]));

    uint8_t function_code = data[MODBUS_INDEX_FUNCTION_CODE];

    uint16_t address = (uint16_t)((uint16_t)(data[MODBUS_INDEX_START_DATA] << 8u) +
                                  (uint16_t)(data[MODBUS_INDEX_START_DATA + 1u]));

    uint16_t qty = (uint16_t)((uint16_t)(data[MODBUS_INDEX_START_DATA + 2u] << 8u) +
                              (uint16_t)(data[MODBUS_INDEX_START_DATA + 3u]));

    modbus_exception_code = 0u;

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
            response_length = modbus_read_data(function_code,
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
            if (qty == MODBUS_WRITE_QUANTITY_1 || 
                qty == MODBUS_WRITE_QUANTITY_2)
            {
                // Check address range
                if (modbus_check_range(address, 
                                       MODBUS_ADDRESS_COIL_START, 
                                       MODBUS_ADDRESS_COIL_END))
                {
                    uint16_t i;
                    uint16_t index_status = MODBUS_SIZE_MBAP + 1u;
                    uint16_t read_value;

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
                    read_value = write_single_coil((i + address), qty);

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
            if (modbus_check_range(qty, 
                                   MODBUS_QUANTITY_HOLDING_REGISTER_START, 
                                   MODBUS_QUANTITY_HOLDING_REGISTER_END))
            {
                // Check address range
                if (modbus_check_range(address, 
                                       MODBUS_ADDRESS_HOLDING_REGISTER_START, 
                                       MODBUS_ADDRESS_HOLDING_REGISTER_END))
                {
                    uint16_t i;
                    uint16_t index_status = MODBUS_SIZE_MBAP + 1u;
                    uint16_t read_value;

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
                    read_value = write_single_register((i + address), qty);

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
            // Any other function code is either not defined or a user defined
            #ifdef USER_DEFINED_FUNCTION_CODE_HOOK
                user_defined_processing_hook();
            #else
                modbus_exception_code = ILLEGAL_FUNCTION;
            #endif
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
 **/
static int modbus_read_data(uint8_t  fn_code,
                     uint16_t qty,
                     uint16_t address,
                     uint16_t address_hi,
                     uint16_t address_lo,
                     uint16_t qty_hi,
                     uint16_t qty_lo,
                     char data[])
{
    int response_length;
    
    // Modbus Application Protocol V1 1B - Section 6.1
    // Check if quantity is within range
    if (modbus_check_range(qty, qty_lo, qty_hi))
    {
        uint16_t end_qty_address = address + qty;

        // Check start and end address
        if (modbus_check_range(address, address_lo, address_hi) &&
            modbus_check_range(end_qty_address, address_lo, address_hi))
        {
            uint16_t i;
            uint16_t index_status = MODBUS_SIZE_MBAP + 2u;
            uint8_t index_byte, index_bit;
            uint16_t read_value;

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
                        read_value = read_coil(i + address);

                        if (read_value == 1u)
                        {
                            data[index_status + index_byte] |=
                                            (1u << index_bit);
                        }
                        else if (read_value == 0u)
                        {
                            data[index_status + index_byte] &=
                                            ~(1u << index_bit);
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
                        read_value = read_discrete_input(i + address);

                        if (read_value == 1u)
                        {
                            data[index_status + index_byte] |=
                                            (1u << index_bit);
                        }
                        else if (read_value == 0u)
                        {
                            data[index_status + index_byte] &=
                                            ~(1u << index_bit);
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
                        read_value = read_holding_register(i + address);

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
                        read_value = read_input_register(i + address);

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

/*=========================================================================*/
