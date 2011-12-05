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

/*---------------------------------------------------------------------------
include files
---------------------------------------------------------------------------*/
#include <platform.h>
#include "application.h"
#include "tcpip_if.h"

/*---------------------------------------------------------------------------
constants
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
ports and clocks
---------------------------------------------------------------------------*/
on stdcore [0]: port buttona = PORT_BUTTON_A;
on stdcore [0]: port buttonb = PORT_BUTTON_B;

on stdcore [0]: port leda = PORT_BUTTON_LED_0;
on stdcore [0]: port ledb = PORT_BUTTON_LED_1;

/*---------------------------------------------------------------------------
typedefs
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
global variables
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
static variables
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
prototypes
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
implementation
---------------------------------------------------------------------------*/

/** =========================================================================
*  application
*
*  \param tcp_svr    Channel that contains data received from TCP
*
**/
void application(chanend tcp_svr)
{
    int led_state = 0;
    xtcp_connection_t conn;

    tcp_reset(tcp_svr);

    // Loop forever processing TCP events
    while (1)
    {
        select
        {
            case xtcp_event(tcp_svr, conn):
            {
                tcp_handle_event(tcp_svr, conn);
                break;
            } // case xtcp_event

            case buttona when pinseq(0xf) :> void:
            {
                leda :> led_state;
                led_state = ~led_state;
                leda <: led_state;
                break;
            } // case buttona
            /*case buttonb :> void:
            {
                ledb :> led_state;
                led_state = ~led_state;
                ledb <: led_state;
                break;
            } // case buttonb*/
        } // select
    } // while(1)
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
char read_coil(unsigned short address)
{
    int led_state;
    leda :> led_state;
    return (char)(led_state);
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
char read_discrete_input(unsigned short address)
{
    return 1;
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
short read_holding_register(unsigned short address)
{
    return 1;
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
short read_input_register(unsigned short address)
{
    return 1;
}

// when pinsneq(0xf) :> void:
/*=========================================================================*/
