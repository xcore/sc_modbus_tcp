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
#include "ethernet_board_support.h"
#include "modbus_tcp_server.h"
#include "modbus_tcp_cmd.h"

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
  on ETHERNET_DEFAULT_TILE: OTP_PORTS_INITIALIZER,
                            ETHERNET_DEFAULT_SMI_INIT,
                            ETHERNET_DEFAULT_MII_INIT_lite,
                            ETHERNET_DEFAULT_RESET_INTERFACE_INIT
};

/*---------------------------------------------------------------------------
 typedefs
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 global variables
 ---------------------------------------------------------------------------*/
// IP Config - change this to suit your network.  Leave with all
// 0 values to use DHCP
xtcp_ipconfig_t ipconfig = {
  { 0, 0, 0, 0 }, // ip address (eg 192,168,0,2)
  { 0, 0, 0, 0 }, // netmask (eg 255,255,255,0)
  { 0, 0, 0, 0 }  // gateway (eg 192,168,0,1)
};

/*---------------------------------------------------------------------------
 static variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 static prototypes
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 Application
 ---------------------------------------------------------------------------*/
void application(chanend c_modbus)
{
  unsigned short mb_data, address, value;
  unsigned short toggle = 0;

  while(1)
  {
    select
    {
      case c_modbus :> mb_data:
      {
        // Received a modbus command

        // Get the address
        c_modbus :> address;

        // Get the value / quantity
        c_modbus :> value;

        switch(mb_data)
        {
          case READ_DISCRETE_INPUTS:
          case READ_INPUT_REGISTER:
          case READ_HOLDING_REGISTERS:
          case WRITE_SINGLE_REGISTER:
          case READ_COILS:
          case WRITE_SINGLE_COIL:
          {
            c_modbus <: toggle;
            break;
          }
          default: break;

        } // switch(mb_data)

        toggle = !toggle;

        break;

      } // case c_modbus :> mb_data:

    } // select

  } // while(1)

}

/*---------------------------------------------------------------------------
 Main Entry Point
 ---------------------------------------------------------------------------*/
int main(void)
{
  chan c_xtcp[1];
  chan c_modbus;

  par
  {
    // The main ethernet/tcp server
    on ETHERNET_DEFAULT_TILE: ethernet_xtcp_server(xtcp_ports,
                                                   ipconfig,
                                                   c_xtcp,
                                                   1);

    // The modbus TCP webserver
    on tile[0]: modbus_tcp_server(c_xtcp[0], c_modbus);
    // The Application
    on tile[0]: application(c_modbus);
  }
  return 0;
}

/*==========================================================================*/
