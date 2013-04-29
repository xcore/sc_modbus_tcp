// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 
 ===========================================================================*/

#ifndef __mbtcp_h__
#define __mbtcp_h__

/*---------------------------------------------------------------------------
 nested include files
 ---------------------------------------------------------------------------*/
#include "xtcp_client.h"
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
 *  Initialize TCP server
 *
 *  \param c_tcp_svr    Channel connecting to XTCP
 *  \return             None
 **/
void mbtcp_init(chanend c_tcp_svr);

/*==========================================================================*/
/**
 *  Handle Events
 *
 *  \param c_tcp_svr    Channel connecting to XTCP
 *  \param c_modbus     Channel connecting to top level application
 *  \param conn         Connection
 *  \return             None
 **/
void mbtcp_handle_event(chanend c_tcp_svr,
                        chanend c_modbus,
                        REFERENCE_PARAM(xtcp_connection_t, conn));

#endif // __mbtcp_h__
/*==========================================================================*/
