// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 Modbus call-back functions
 ===========================================================================*/

#ifndef _modbus_cb_functions_h_
#define _modbus_cb_functions_h_

/*---------------------------------------------------------------------------
 nested include files
 ---------------------------------------------------------------------------*/

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
 *  read coil
 *
 *  \param address  address of the coil
 *  \return         coil value
 **/
unsigned short read_coils(unsigned short address);
/*==========================================================================*/
/**
 *  read discrete input
 *
 *  \param address  address of the discrete input
 *  \return         discrete input value
 **/
unsigned short read_discrete_inputs(unsigned short address);
/*==========================================================================*/
/**
 *  read holding register
 *
 *  \param address  address of the holding register
 *  \return         holding register value
 **/
unsigned short read_holding_registers(unsigned short address);
/*==========================================================================*/
/**
 *  read input register
 *
 *  \param address  address of the input register
 *  \return         input register value
 **/
unsigned short read_input_registers(unsigned short address);
/*==========================================================================*/
/**
 *  write single coil
 *
 *  \param address  address of the coil
 *  \param value    value to write
 *  \return         0 is failure, 1 is ok
 **/
unsigned short write_single_coil(unsigned short address, unsigned short value);
/*==========================================================================*/
/**
 *  write single register
 *
 *  \param address  address of the register
 *  \param value    value to write
 *  \return         0 is failure, 1 is ok
 **/
unsigned short write_single_register(unsigned short address, unsigned short value);

#endif // _modbus_cb_functions_h_
/*==========================================================================*/
