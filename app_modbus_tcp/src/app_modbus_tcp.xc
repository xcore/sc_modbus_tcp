// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----

 ===========================================================================*/

/*---------------------------------------------------------------------------
 include files
 ---------------------------------------------------------------------------*/
#include <platform.h>
#include "xtcp.h"
#include "i2c.h"
#include "ethernet_board_support.h"
#include "xhttpd.h"
#include "modbus_cb_functions.h"

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 ports and clocks
 ---------------------------------------------------------------------------*/
// These intializers are taken from the ethernet_board_support.h header for
// XMOS dev boards. If you are using a different board you will need to
// supply explicit port structure intializers for these values
ethernet_xtcp_ports_t xtcp_ports =
{
  on tile[1]: OTP_PORTS_INITIALIZER,
              ETHERNET_DEFAULT_SMI_INIT,
              ETHERNET_DEFAULT_MII_INIT_lite,
              ETHERNET_DEFAULT_RESET_INTERFACE_INIT
};

// GPIO slice ports
on tile[1]: r_i2c p_i2c = {XS1_PORT_1F, XS1_PORT_1B, 1000};
on tile[1]: port p_led = XS1_PORT_4A;
on tile[1]: port p_button = XS1_PORT_4C;

/*---------------------------------------------------------------------------
 typedefs
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 global variables
 ---------------------------------------------------------------------------*/
// IP Config - change this to suit your network. All 0s to use DHCP
xtcp_ipconfig_t ipconfig = {
  { 169, 254, 231, 27 },
  { 255, 255, 0, 0 },
  { 0, 0, 0, 0 }
};

// Global button status
unsigned short button_status = 0;

/*---------------------------------------------------------------------------
 static variables
 ---------------------------------------------------------------------------*/
//Temperature to ADC look up table
static int TEMPERATURE_LUT[][2] = { {-10, 850}, {-5, 800}, {0, 750}, {5, 700},
                                    {10, 650}, {15, 600}, {20, 550}, {25, 500},
                                    {30, 450}, {35, 400}, {40, 350}, {45, 300},
                                    {50, 250}, {55, 230}, {60, 210}
};

/*---------------------------------------------------------------------------
 static prototypes
 ---------------------------------------------------------------------------*/

/*==========================================================================*/
/**
 *  Read temperature value from the sensor using I2C
 *
 *  \param p_i2c  I2C ports that connects to the temperature sensor
 *  \return       Current recorded temperature on the sensor
 **/
static int read_temperature(r_i2c &p_i2c)
{
  int adc_value;
  int i = 0, x1, y1, x2, y2, temperature;
  unsigned char i2c_data[2];

  i2c_master_rx(0x28, i2c_data, sizeof(i2c_data), p_i2c);
  i2c_data[0] = i2c_data[0] & 0x0F;
  adc_value = (i2c_data[0] << 6) | (i2c_data[1] >> 2);

  while(adc_value < TEMPERATURE_LUT[i][1])  { i++; }

  //Calculate Linear interpolation using: y = y1+(x-x1)*(y2-y1)/(x2-x1)
  x1 = TEMPERATURE_LUT[i - 1][1];
  y1 = TEMPERATURE_LUT[i - 1][0];
  x2 = TEMPERATURE_LUT[i][1];
  y2 = TEMPERATURE_LUT[i][0];
  temperature = y1 + (((adc_value - x1) * (y2 - y1)) / (x2 - x1));

  return temperature;
}

/*---------------------------------------------------------------------------
 Main Entry Point
 ---------------------------------------------------------------------------*/
int main(void)
{
  chan c_xtcp[1];

  par
  {
    // The main ethernet/tcp server
    on tile[1]: ethernet_xtcp_server(xtcp_ports, ipconfig, c_xtcp, 1);

    // The webserver, GPIO handler
    on tile[1]:
    {
      // Initialize temperature sensor
      unsigned char i2c_register[1] = {0x13};
      i2c_master_write_reg(0x28, 0x00, i2c_register, 1, p_i2c);

      // Start the Webserver and Button listener
      xhttpd(c_xtcp[0], p_button);
    }

  } // par
  return 0;
}

/*---------------------------------------------------------------------------
 Modbus function callbacks
 ---------------------------------------------------------------------------*/

unsigned short read_coils(unsigned short address)
{
  /**
   * In Modbus Conf file: Coil start is 0 and End is 3
   * LEDs on the GPIO slice are imitated as coils.
   * Modbus Coil Address 0 = LED0
   * Modbus Coil Address 1 = LED1
   * Modbus Coil Address 2 = LED2
   * Modbus Coil Address 3 = LED3
   * Other coil addresses return 0 (device failures)
   */
  int led_status;

  p_led :> led_status;
  if(led_status & (1 << address)) { return 1; }
  return 0;
}

unsigned short read_discrete_inputs(unsigned short address)
{
  /*
   * Button SW1 is at address 0
   * Button SW2 is at address 1
   */
  unsigned short rtnval = button_status;
  button_status &= ~(1 << address);
  if(rtnval & (1 << address)) { return 1; }
  return 0;
}

unsigned short read_holding_registers(unsigned short address)
{
  // Not implemented, return 0
  return 0;
}

unsigned short read_input_registers(unsigned short address)
{
  return (unsigned short)(read_temperature(p_i2c));
}

unsigned short write_single_coil(unsigned short address, unsigned short value)
{
  /**
   * Write Single coil actually toggles the state of LED (imitated as coils).
   * Modbus Coil Address 0 = LED0
   * Modbus Coil Address 1 = LED1
   * Modbus Coil Address 2 = LED2
   * Modbus Coil Address 3 = LED3
   * Other coil addresses return 0 (device failures)
   */
  int led_status;

  p_led :> led_status;

  if(led_status & (1 << address))
  {
    led_status &= ~(1 << address);
  }
  else
  {
    led_status |= (1 << address);
  }

  p_led <: led_status;
  return 1;
}

unsigned short write_single_register(unsigned short address, unsigned short value)
{
  // Not implemented, return 0
  return 0;
}

/*==========================================================================*/
