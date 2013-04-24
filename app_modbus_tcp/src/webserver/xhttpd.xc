// Copyright (c) 2011, XMOS Ltd., All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*===========================================================================
 Info
 ----
 The main webserver thread.

 ===========================================================================*/

/*---------------------------------------------------------------------------
 include files
 ---------------------------------------------------------------------------*/
#include <xs1.h>
#include "httpd.h"
#include "xtcp_client.h"

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/
// Timer interval to scan button events
#define DEBOUNCE_INTERVAL     XS1_TIMER_HZ/50
#define BUTTON_1_PRESS_VALUE  0x2

/*---------------------------------------------------------------------------
 ports and clocks
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 typedefs
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 global variables
 ---------------------------------------------------------------------------*/
extern unsigned short button_status;

/*---------------------------------------------------------------------------
 static variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 static prototypes
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 xhttpd
 ---------------------------------------------------------------------------*/
void xhttpd(chanend c_tcp_svr, port p_button)
{
  int scan_button_flag = 1;
  unsigned button_state_1 = 0;
  unsigned button_state_2 = 0;
  timer t_scan_button_flag;
  unsigned time;
  xtcp_connection_t conn;

  // Initiate the HTTP state
  httpd_init(c_tcp_svr);

  set_port_drive_low(p_button);
  t_scan_button_flag :> time;
  p_button :> button_state_1;

  while(1)
  {
    select
    {
      // Listen to XTCP events
      case xtcp_event(c_tcp_svr, conn):
      {
        httpd_handle_event(c_tcp_svr, conn);
        break;
      }

      //::Button Scan Start
      case scan_button_flag=> p_button when pinsneq(button_state_1) :> button_state_1 :
      {
        t_scan_button_flag :> time;
        scan_button_flag = 0;
        break;
      }

      case !scan_button_flag => t_scan_button_flag when timerafter(time + DEBOUNCE_INTERVAL) :> void:
      {
        p_button :> button_state_2;
        if(button_state_1 == button_state_2)
        {
          if(button_state_1 == BUTTON_1_PRESS_VALUE)
          {
            button_status |= 0x01;
          }
          if(button_state_2 == BUTTON_1_PRESS_VALUE-1)
          {
            button_status |= 0x02;
          }
        }
        scan_button_flag = 1;
        break;
      }
      //::Button Scan End

    }
  }
}

/*==========================================================================*/
