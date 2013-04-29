// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 
 ===========================================================================*/

#ifndef __mb_util_h__
#define __mb_util_h__

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
 *  Send commands to the top level application in order to read/write device
 *  values. This function will send:
 *  unsigned char: Modbus command
 *  unsigned short: Address to read/write
 *  unsigned short: Value to write (sent always. on read command, this is 0)
 *
 *  And expects:
 *  unsigned short: value (for Read) or status (for write)
 *
 *  \param c_modbus   Channel connecting to top leel application
 *  \param fn_code    Modbus Function code
 *  \param address    Device address
 *  \param value      Value
 *  \return           Value/Status
 **/
unsigned short access_external_device(chanend c_modbus,
                                      unsigned char fn_code,
                                      unsigned short address,
                                      unsigned short value);

#endif // __mb_util_h__
/*==========================================================================*/
