/*==============================================================================
Filename:
Project :
Author  :
Version :
Purpose
--------------------------------------------------------------------------------

Version History
--------------------------------------------------------------------------------

License
--------------------------------------------------------------------------------
Copyright (c) 2011, XMOS Ltd, All rights reserved
This software is freely distributable under a derivative of the University of 
Illinois/NCSA Open Source License posted in LICENSE.txt and at 
<http://github.xcore.com/>
==============================================================================*/

/*
 ============================================================================
 Name        : xc2-led.xc
 Description : Illuminate an LED on the XC-2 card 
 ============================================================================
 */

#include <platform.h>

out port x0ledB = PORT_LED_0_1;

static char led = 0u;

void vin_led() {
  x0ledB <: led & 0x01;
  led = ~led;
}