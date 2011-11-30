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
#include <string.h>
#include <print.h>
#include "tcpip_if.h"
#include "xtcp_client.h"
#include "pkt_processing.h"

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
static variables
---------------------------------------------------------------------------*/
static char tcp_data[SIZE_TCP_DATA];
static char connection_state[NUM_CONNECTIONS];
static char have_stuff_to_send = 0u;

/*---------------------------------------------------------------------------
prototypes
---------------------------------------------------------------------------*/
static void tcp_send(chanend tcp_svr, unsigned event_type);
static void tcp_new_connection(chanend tcp_svr, xtcp_connection_t *conn);
static void tcp_recv(chanend tcp_svr, xtcp_connection_t *conn);

/*---------------------------------------------------------------------------
implementation
---------------------------------------------------------------------------*/

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
static void tcp_recv(chanend tcp_svr, xtcp_connection_t *conn)
{
    int length;
    int temp;

    // Receive the data from the TCP stack
    length = xtcp_recv(tcp_svr, tcp_data);

    temp = modbus_tcp_process_frame(tcp_data, length);
    //
    //
    //
    //
    //
    //check_adu = mbtcp_parse_adu();
    //
    //
    //
    //
    //

    have_stuff_to_send = 0u; //1u;
    xtcp_init_send(tcp_svr, conn);
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
static void tcp_send(chanend tcp_svr, unsigned event_type)
{
    // an error occurred in send so we should resend the existing data
    if (event_type == XTCP_RESEND_DATA)
    {
        xtcp_send(tcp_svr, tcp_data, 8u);
    }
    else
    {
        if (have_stuff_to_send)
        {
            xtcp_send(tcp_svr, tcp_data, 8u);
            have_stuff_to_send = 0;
        }
        else
        {
            xtcp_complete_send(tcp_svr);
        }
    }
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
static void tcp_new_connection(chanend tcp_svr, xtcp_connection_t *conn)
{
    int i;

    // Try and find an empty connection slot
    for (i = 0u; i < NUM_CONNECTIONS; i++)
    {
        if (!connection_state[i])
        {
            // found a free connection, exit this for loop
            break;
        }
    }

    // If no free connection slots were found, abort the connection
    if ( i == NUM_CONNECTIONS )
    {
        xtcp_abort(tcp_svr, conn);
    }
    // Otherwise, assign the connection to a slot
    else
    {
        connection_state[i] = 1u;
        xtcp_set_connection_appstate(tcp_svr,
                                     conn,
                                     (xtcp_appstate_t) &connection_state[i]);
    }
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
void tcp_reset(chanend tcp_svr)
{
    unsigned int index;

    // flush modbus frame
    for(index = 0u; index < SIZE_TCP_DATA; index++)
    {
        tcp_data[index] = 0u;
    }

    // flush all connections
    for(index = 0u; index < NUM_CONNECTIONS; index++)
    {
        connection_state[index] = 0u;
    }

    xtcp_listen(tcp_svr, LISTEN_PORT, XTCP_PROTOCOL_TCP);
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
void tcp_handle_event(chanend tcp_svr, xtcp_connection_t *conn)
{
    // Ignore already handled events
    switch (conn->event)
    {
    case XTCP_IFUP:
    case XTCP_IFDOWN:
    case XTCP_ALREADY_HANDLED:
        return;
    default:
        break;
    }

    if (conn->local_port == LISTEN_PORT) {
        switch (conn->event)
        {
        case XTCP_NEW_CONNECTION:
            tcp_new_connection(tcp_svr, conn);
            break;
        case XTCP_RECV_DATA:
            tcp_recv(tcp_svr, conn);
            break;
        case XTCP_SENT_DATA:
        case XTCP_REQUEST_DATA:
        case XTCP_RESEND_DATA:
            tcp_send(tcp_svr, conn->event);
            break;
        case XTCP_TIMED_OUT:
        case XTCP_ABORTED:
        case XTCP_CLOSED:
            tcp_reset(tcp_svr);
            break;
        default: break;
        }
        conn->event = XTCP_ALREADY_HANDLED;
    }
    return;
}

/*=========================================================================*/

