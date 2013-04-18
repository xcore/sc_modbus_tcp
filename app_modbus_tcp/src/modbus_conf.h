// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----

 ===========================================================================*/

#ifndef _modbus_conf_h_
#define _modbus_conf_h_

/*---------------------------------------------------------------------------
 nested include files
 ---------------------------------------------------------------------------*/
#include "modbus_cb_functions.h"

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/
#define MODBUS_ADDRESS_COIL_START                      0x0000
#define MODBUS_ADDRESS_COIL_END                        0x0004
#define MODBUS_ADDRESS_HOLDING_REGISTER_START          0x0000
#define MODBUS_ADDRESS_HOLDING_REGISTER_END            0x0001
#define MODBUS_ADDRESS_INPUT_REGISTER_START            0x0000
#define MODBUS_ADDRESS_INPUT_REGISTER_END              0x0001
#define MODBUS_ADDRESS_DISCRETE_INPUT_START            0x0000
#define MODBUS_ADDRESS_DISCRETE_INPUT_END              0x0002
#define MODBUS_QUANTITY_COIL_START                     0x0001
#define MODBUS_QUANTITY_COIL_END                       0x0004
#define MODBUS_QUANTITY_HOLDING_REGISTER_START         0x0001
#define MODBUS_QUANTITY_HOLDING_REGISTER_END           0x0001
#define MODBUS_QUANTITY_INPUT_REGISTER_START           0x0001
#define MODBUS_QUANTITY_INPUT_REGISTER_END             0x0001
#define MODBUS_QUANTITY_DISCRETE_INPUT_START           0x0001
#define MODBUS_QUANTITY_DISCRETE_INPUT_END             0x0002
#define MODBUS_WRITE_QUANTITY_1                        0x0000
#define MODBUS_WRITE_QUANTITY_2                        0xFF00

/*---------------------------------------------------------------------------
 function callbacks
 ---------------------------------------------------------------------------*/
#define MB_READ_COILS               read_coils
#define MB_READ_DISCRETE_INPUTS     read_discrete_inputs
#define MB_READ_HOLDING_REGISTERS   read_holding_registers
#define MB_READ_INPUT_REGISTERS     read_input_registers
#define MB_WRITE_SINGLE_COIL        write_single_coil
#define MB_WRITE_SINGLE_REGISTER    write_single_register

/*---------------------------------------------------------------------------
 global variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 extern variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 prototypes
 ---------------------------------------------------------------------------*/

#endif // _modbus_conf_h_
/*==========================================================================*/
