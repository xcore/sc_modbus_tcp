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

#include <platform.h>
#include "modbus_tcp_server.h"

static void toggle_led(out port led_port, REFERENCE_PARAM(char, led_val));

out port pled00 = PORT_LED_0_0;
out port pled01 = PORT_LED_0_1;

static char led00 = 0u;
static char led01 = 0u;

int vin_led(char adu[])
{
   int function_code;

   function_code = adu[INDEX_FUNCTION_CODE];

   switch(function_code)
   {
   case READ_DISCRETE_INPUTS:
      {
         toggle_led(pled00, led00);
         break;
      }
   case READ_COILS:
      {
         toggle_led(pled01, led01);
         break;
      }
   case WRITE_SINGLE_COIL:
      {
         toggle_led(pled00, led00);
         break;
      }
   case WRITE_MULTIPLE_COILS:
      {
         toggle_led(pled01, led01);
         break;
      }
   case READ_INPUT_REGISTER:
      {
         toggle_led(pled00, led00);
         break;
      }
   case READ_HOLDING_REGISTERS:
      {
         toggle_led(pled01, led01);
         break;
      }
   default: break;
   }

   return 0;
}

static void toggle_led(out port led_port, REFERENCE_PARAM(char, led_val))
{
   led_port <: led_val & 0x01u;
   led_val = ~led_val;
}