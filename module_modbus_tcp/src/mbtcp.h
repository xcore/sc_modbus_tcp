// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 
 ===========================================================================*/

#ifndef _mbtcp_h_
#define _mbtcp_h_

/*---------------------------------------------------------------------------
 nested include files
 ---------------------------------------------------------------------------*/
#include "xtcp_client.h"

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
 *  Initialize Modbus TCP webserver
 *
 *  \param c_tcp_svr  server channel connecting to ethernet component
 *  \return None
 **/
void modbus_tcp_init(chanend c_tcp_svr);

/*==========================================================================*/
/**
 *  Handle Modbus TCP events
 *
 *  \param c_tcp_svr  server channel connecting to ethernet component
 *  \param c_modbus   modbus data/cmd channel connecting to application
 *  \param conn       XTCP connection
 *  \return None
 **/
void modbus_tcp_handle_event(chanend c_tcp_svr,
                             chanend c_modbus,
                             REFERENCE_PARAM(xtcp_connection_t, conn));

#endif // _mbtcp_h_
/*==========================================================================*/
