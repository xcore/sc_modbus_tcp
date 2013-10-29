// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----

 ===========================================================================*/

#ifndef __modbus_tcp_h__
#define __modbus_tcp_h__

/*---------------------------------------------------------------------------
 nested include files
 ---------------------------------------------------------------------------*/
#include "ethernet_board_support.h"
#include "xtcp.h"
#include "mb_codes.h"
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
 *  Modbus TCP Server. This Modbus Slave task must be called from the main 
 *  user application. It instantiates Ethernet and TCP server components to 
 *  receive Modbus TCP commands (from a Modbus Master - a TCP client) over 
 *  the TCP layer.
 *
 *  It processes Modbus commands and requests user application for data. These
 *  requests to user are made over ``c_modbus``. The data in c_modbus is of 
 *  the following format:
 *  
 *  It sends:
 *  unsigned char: Modbus command
 *  unsigned short: Address to read/write
 *  unsigned short: Value to write (sent always. on read command, this is 0)
 *
 *  And expects:
 *  unsigned short: value (for Read) or status (for write) 
 *  
 *  After receiving request data from user application, it sends the response
 *  back to Modbus master over TCP layer.
 *
 *  \param c_modbus   Channel connecting to top-level application
 *  \param xtcp_ports Ethernet ports to use
 *  \param ipconfig   IP configuration to use
 *  \return None
 **/
void modbus_tcp_server(chanend c_modbus,
                       REFERENCE_PARAM(ethernet_xtcp_ports_t, xtcp_ports),
                       REFERENCE_PARAM(xtcp_ipconfig_t, ipconfig));

#endif // __modbus_tcp_h__
/*==========================================================================*/
