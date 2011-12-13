// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

#ifndef MODBUS_PKT_PROCESSING_H_
#define MODBUS_PKT_PROCESSING_H_
/*===========================================================================
Filename:
Project :
Author  :
Version :
Purpose
-----------------------------------------------------------------------------


===========================================================================*/

/*---------------------------------------------------------------------------
nested include files
---------------------------------------------------------------------------*/
#include "xtcp_client.h"

#ifdef __modbus_config_h_exists__
   #include <modbus_config.h>
#endif

/*---------------------------------------------------------------------------
constants
---------------------------------------------------------------------------*/
// User defines addresses and quantity limits
#ifndef MODBUS_ADDRESS_COIL_START
    #define MODBUS_ADDRESS_COIL_START                      0x0000
#endif
#ifndef MODBUS_ADDRESS_COIL_END
    #define MODBUS_ADDRESS_COIL_END                        0xFFFF
#endif
#ifndef MODBUS_ADDRESS_HOLDING_REGISTER_START
    #define MODBUS_ADDRESS_HOLDING_REGISTER_START          0x0000
#endif
#ifndef MODBUS_ADDRESS_HOLDING_REGISTER_END
    #define MODBUS_ADDRESS_HOLDING_REGISTER_END            0xFFFF
#endif
#ifndef MODBUS_ADDRESS_INPUT_REGISTER_START
    #define MODBUS_ADDRESS_INPUT_REGISTER_START            0x0000
#endif
#ifndef MODBUS_ADDRESS_INPUT_REGISTER_END
    #define MODBUS_ADDRESS_INPUT_REGISTER_END              0xFFFF
#endif
#ifndef MODBUS_ADDRESS_DISCRETE_INPUT_START
    #define MODBUS_ADDRESS_DISCRETE_INPUT_START            0x0000
#endif
#ifndef MODBUS_ADDRESS_DISCRETE_INPUT_END
    #define MODBUS_ADDRESS_DISCRETE_INPUT_END              0xFFFF
#endif
#ifndef MODBUS_QUANTITY_COIL_START
    #define MODBUS_QUANTITY_COIL_START                     0x0001
#endif
#ifndef MODBUS_QUANTITY_COIL_END
    #define MODBUS_QUANTITY_COIL_END                       0x07D0
#endif
#ifndef MODBUS_QUANTITY_HOLDING_REGISTER_START
    #define MODBUS_QUANTITY_HOLDING_REGISTER_START         0x0001
#endif
#ifndef MODBUS_QUANTITY_HOLDING_REGISTER_END
    #define MODBUS_QUANTITY_HOLDING_REGISTER_END           0x007D
#endif
#ifndef MODBUS_QUANTITY_INPUT_REGISTER_START
    #define MODBUS_QUANTITY_INPUT_REGISTER_START           0x0001
#endif
#ifndef MODBUS_QUANTITY_INPUT_REGISTER_END
    #define MODBUS_QUANTITY_INPUT_REGISTER_END             0x007D
#endif
#ifndef MODBUS_QUANTITY_DISCRETE_INPUT_START
    #define MODBUS_QUANTITY_DISCRETE_INPUT_START           0x0001
#endif
#ifndef MODBUS_QUANTITY_DISCRETE_INPUT_END
    #define MODBUS_QUANTITY_DISCRETE_INPUT_END             0x07D0
#endif
#ifndef MODBUS_WRITE_QUANTITY_1
   #define MODBUS_WRITE_QUANTITY_1                         0x0000
#endif
#ifndef MODBUS_WRITE_QUANTITY_2
   #define MODBUS_WRITE_QUANTITY_2                         0xFF00
#endif

// Public Function Code Definitions
// Comment out functions that are not supported
#define READ_DISCRETE_INPUTS                    0x02
#define READ_COILS                              0x01
#define WRITE_SINGLE_COIL                       0x05
//#define WRITE_MULTIPLE_COILS                    0x0F
#define READ_INPUT_REGISTER                     0x04
#define READ_HOLDING_REGISTERS                  0x03
#define WRITE_SINGLE_REGISTER                   0x06
//#define WRITE_MULTIPLE_REGISTERS                0x10
//#define READ_WRITE_MULTIPLE_REGISTERS           0x17
//#define MASK_WRITE_REGISTER                     0x16
//#define READ_FIFO_QUEUE                         0x18
//#define READ_FILE_RECORD                        0x14
//#define WRITE_FILE_RECORD                       0x15
//#define READ_EXCEPTION_STATUS                   0x07
//#define DIAGNOSTIC                              0x08
//#define GET_COM_EVENT_COUNTER                   0x0B
//#define GET_COM_EVENT_LOG                       0x0C
//#define REPORT_SLAVE_ID                         0x11
//#define READ_DEVICE_IDENTIFICATION              0x2B
//#define UMAS_FILE_TRANSFER                      0x5A

// Modbus protocol ID and data sizes
#define MODBUS_PROTOCOL_IDENTIFIER              0x0000u
#define MODBUS_UNIT_IDENTIFIER                  0x01u
#define MODBUS_SIZE_DATA                        253u
#define MODBUS_SIZE_MBAP                        7u

// Big Endian format in Modbus data
#define MODBUS_INDEX_TRANSACTION_ID                    0u
#define MODBUS_INDEX_PROTOCOL_ID                       2u
#define MODBUS_INDEX_LENGTH_FIELD                      4u
#define MODBUS_INDEX_UNIT_ID                           6u
#define MODBUS_INDEX_FUNCTION_CODE                     7u
#define MODBUS_INDEX_START_DATA                        8u

// Diagnostic (0x08) Sub Function Code
#define RETURN_QUERY_DATA                       0x00
#define RESTART_COMMUNICATIONS_OPTION           0x01
#define RETURN_DIAGNOSTIC_REGISTER              0x02
#define CHANGE_ASCII_INPUT_DELIMITER            0x03
#define FORCE_LISTEN_ONLY_MODE                  0x04
#define CLEAR_COUNTERS_AND_DIAGNOSTIC_REGISTER  0x0A
#define RETURN_BUS_MESSAGE_COUNT                0x0B
#define RETURN_BUS_COMMUNICATION_ERROR_COUNT    0x0C
#define RETURN_BUS_EXCEPTION_ERROR_COUNT        0x0D
#define RETURN_SLAVE_MESSAGE_COUNT              0x0E
#define RETURN_SLAVE_NO_RESPONSE_COUNT          0x0F
#define RETURN_SLAVE_NAK_COUNT                  0x10
#define RETURN_SLAVE_BUSY_COUNT                 0x11
#define RETURN_BUS_CHARACTER_OVERRUN_COUNT      0x12
#define CLEAR_OVERRRUN_COUNTER_AND_FLAG         0x14

// Modbus Exception Codes
#define ILLEGAL_FUNCTION                        0x01
#define ILLEGAL_DATA_ADDRESS                    0x02
#define ILLEGAL_DATA_VALUE                      0x03
#define SLAVE_DEVICE_FAILURE                    0x04
#define ACKNOWLEDGE                             0x05
#define SLAVE_DEVICE_BUSY                       0x06
#define MEMORY_PARITY_ERROR                     0x08
#define GATEWAY_PATH_UNAVAILABLE                0x0A
#define GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND 0x0B

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
prototypes
---------------------------------------------------------------------------*/
int modbus_process_frame(char data[], int length);
char modbus_get_status(void);

#endif // MODBUS_PKT_PROCESSING_H_
/*=========================================================================*/

