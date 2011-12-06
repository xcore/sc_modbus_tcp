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

#ifndef APPLICATION_H_
#define APPLICATION_H_

/*---------------------------------------------------------------------------
nested include files
---------------------------------------------------------------------------*/
#include "xtcp_client.h"

/*---------------------------------------------------------------------------
constants
---------------------------------------------------------------------------*/

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
void application(chanend tcp_svr);

char read_coil(unsigned short address);
char read_discrete_input(unsigned short address);
short read_holding_register(unsigned short address);
short read_input_register(unsigned short address);
short write_single_coil(unsigned short address, short value);
short write_single_register(unsigned short address, short value);

#endif // APPLICATION_H_

/*=========================================================================*/
