// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----

 ===========================================================================*/

#ifndef __ethernet_conf_h__
#define __ethernet_conf_h__

/*---------------------------------------------------------------------------
 nested include files
 ---------------------------------------------------------------------------*/
#ifdef __modbus_tcp_conf_h_exists__
#include "modbus_tcp_conf.h"
#endif

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/
/* Get where the Ethernet slice is connected on the sliceKIT core board and
 * define appropriate Ethernet confs. If nothing is mentioned in the
 * modbus_tcp_conf.h about Ethernet slice location, CIRCLE slot is assumed.
 */
#ifdef MODBUS_TCP_ETHERNET_SLOT_SQUARE
  #define ETHERNET_USE_SQUARE_SLOT
#endif

#ifdef MODBUS_TCP_ETHERNET_SLOT_STAR
  #define ETHERNET_USE_STAR_SLOT
#endif

#ifdef MODBUS_TCP_ETHERNET_SLOT_TRIANGLE
  #define ETHERNET_USE_TRIANGLE_SLOT
#endif

#ifdef MODBUS_TCP_ETHERNET_SLOT_CIRCLE
  #define ETHERNET_USE_CIRCLE_SLOT
#endif

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


#endif // __ethernet_conf_h__
/*==========================================================================*/
