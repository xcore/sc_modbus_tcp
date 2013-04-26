// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 
 ===========================================================================*/

#ifndef __mbtcp_parser_h__
#define __mbtcp_parser_h__

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
int modbus_tcp_parse_request(chanend c_modbus,
                             REFERENCE_PARAM(char, data),
                             int len);

#endif // __mbtcp_parser_h__
/*==========================================================================*/
