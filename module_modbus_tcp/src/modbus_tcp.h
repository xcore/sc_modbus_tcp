// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 
 ===========================================================================*/

#ifndef _modbus_tcp_h_
#define _modbus_tcp_h_

/*---------------------------------------------------------------------------
 nested include files
 ---------------------------------------------------------------------------*/
#include <xccompat.h>

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 typedefs
 ---------------------------------------------------------------------------*/
/*
 * Modbus TCP exception codes.
 */
typedef enum mb_tcp_exception_codes_t_
{
  ILLEGAL_FUNCTION = 0x01,
  ILLEGAL_DATA_ADDRESS = 0x02,
  ILLEGAL_DATA_VALUE = 0x03,
  SLAVE_DEVICE_FAILURE = 0x04,
  ACKNOWLEDGE = 0x05,
  SLAVE_DEVICE_BUSY = 0x06,
  MEMORY_PARITY_ERROR = 0x08,
  GATEWAY_PATH_UNAVAILABLE = 0x0A,
  GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND = 0x0B,
} mb_tcp_exception_code_t;

/*---------------------------------------------------------------------------
 global variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 extern variables
 ---------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------
 prototypes
 ---------------------------------------------------------------------------*/

/*==========================================================================*/
/**
 *  Parses Modbus commands, does a call-back to user application for values
 *  and forms Modbus TCP response. The call-back functions are defined in
 *  ``modbus_conf.h`` file.
 *
 *  \param data   data received from the TCP layer
 *  \param len    length of data received
 *  \return length of Modbus TCP response
 **/
int modbus_tcp_parse_request(REFERENCE_PARAM(char, data), int len);

#endif // _modbus_tcp_h_
/*==========================================================================*/
