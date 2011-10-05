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
#include "system.h"
#include "xtcp_client.h"

/*------------------------------------------------------------------------------
Extern Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Manifest constants
------------------------------------------------------------------------------*/
// Public Function Code Definitions
#define READ_DISCRETE_INPUTS                    0x02
#define READ_COILS                              0x01
#define WRITE_SINGLE_COIL                       0x05
#define WRITE_MULTIPLE_COILS                    0x0F
#define READ_INPUT_REGISTER                     0x04
#define READ_HOLDING_REGISTERS                  0x03
#define WRITE_SINGLE_REGISTER                   0x06
#define WRITE_MULTIPLE_REGISTERS                0x10
#define READ_WRITE_MULTIPLE_REGISTERS           0x17
#define MASK_WRITE_REGISTER                     0x16
#define READ_FIFO_QUEUE                         0x18
#define READ_FILE_RECORD                        0x14
#define WRITE_FILE_RECORD                       0x15
#define READ_EXCEPTION_STATUS                   0x07
#define DIAGNOSTIC                              0x08
#define GET_COM_EVENT_COUNTER                   0x0B
#define GET_COM_EVENT_LOG                       0x0C
#define REPORT_SLAVE_ID                         0x11
#define READ_DEVICE_IDENTIFICATION              0x2B
#define UMAS_FILE_TRANSFER                      0x5A

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

// User Defines
#define ALL_OK                                  0x00u
#define MODBUS_PROTOCOL_IDENTIFIER              0x0000u
#define MODBUS_UNIT_IDENTIFIER                  0x01u

#define NUM_MAX_REGISTERS                       0x007D
#define NUM_MIN_REGISTERS                       0x0001
#define NUM_MAX_ADDRESS                         0x0011
#define NUM_MIN_ADDRESS                         0x7FFF
#define SIZE_MODBUS_MBAP                        7u
#define SIZE_MODBUS_DATA                        253u
#define SIZE_MODBUS_ADU                         (SIZE_MODBUS_MBAP + SIZE_MODBUS_DATA)
#define INDEX_PDU                               (SIZE_MODBUS_MBAP)

// Big Endian format
#define INDEX_HI_TRANSACTION_ID                 0u
#define INDEX_HI_PROTOCOL_ID                    2u
#define INDEX_HI_LENGTH_FIELD                   4u
#define INDEX_UNIT_ID                           6u
#define INDEX_FUNCTION_CODE                     7u
#define INDEX_START_DATA                        8u

#define PORT_LISTEN                             80

/*------------------------------------------------------------------------------
Typedefs
------------------------------------------------------------------------------*/
typedef struct
{
   uint16_t transaction_id;
   uint16_t protocol_id;
   uint16_t length_field;
   uint8_t  unit_id;
   uint8_t  function_code;
   uint8_t  data[SIZE_MODBUS_DATA];
} s_modbus_tcp_adu_t;

/*------------------------------------------------------------------------------
Global Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Static Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Prototypes
------------------------------------------------------------------------------*/
uint8_t mbtcp_handle_event(chanend c_xtcp,
                           xtcp_connection_t *conn,
                           chanend c_user);

/*============================================================================*/
