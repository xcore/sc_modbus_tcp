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
#include "modbus_pkt_processing.h"
#include "modbus_tcpip_if.h"
#include "xtcp_client.h"

/*---------------------------------------------------------------------------
constants
---------------------------------------------------------------------------*/
#define MODBUS_SIZE_TCP_DATA               260
#define MODBUS_LISTEN_PORT                 502
#define MODBUS_NUM_CONNECTIONS             1

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
static char modbus_data[MODBUS_SIZE_TCP_DATA];
static char modbus_connection_state[MODBUS_NUM_CONNECTIONS];
static char modbus_have_stuff_to_send = 0u;
static int  modbus_response_length;

/*---------------------------------------------------------------------------
prototypes
---------------------------------------------------------------------------*/
static void modbus_tcp_send(chanend c_modbus, unsigned event_type);
static void modbus_tcp_new_connection(chanend c_modbus, xtcp_connection_t *conn);
static void modbus_tcp_recv(chanend c_modbus, xtcp_connection_t *conn);

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
static void modbus_tcp_recv(chanend c_modbus, xtcp_connection_t *conn)
{
    int length;

    // Receive the data from the TCP stack
    length = xtcp_recv(c_modbus, modbus_data);

    modbus_response_length = modbus_process_frame(modbus_data, length);

    if (modbus_response_length)
    {
        modbus_have_stuff_to_send = 1u;
    }
    else
    {
        modbus_have_stuff_to_send = 0u;
    }
    xtcp_init_send(c_modbus, conn);
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
static void modbus_tcp_send(chanend c_modbus, unsigned event_type)
{
    // an error occurred in send so we should resend the existing data
    if (event_type == XTCP_RESEND_DATA)
    {
        xtcp_send(c_modbus, modbus_data, modbus_response_length);
    }
    else
    {
        if (modbus_have_stuff_to_send)
        {
            xtcp_send(c_modbus, modbus_data, modbus_response_length);
            modbus_have_stuff_to_send = 0;
        }
        else
        {
            xtcp_complete_send(c_modbus);
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
static void modbus_tcp_new_connection(chanend c_modbus, xtcp_connection_t *conn)
{
    int i;

    // Try and find an empty connection slot
    for (i = 0u; i < MODBUS_NUM_CONNECTIONS; i++)
    {
        if (!modbus_connection_state[i])
        {
            // found a free connection, exit this for loop
            break;
        }
    }

    // If no free connection slots were found, abort the connection
    if ( i == MODBUS_NUM_CONNECTIONS )
    {
        xtcp_abort(c_modbus, conn);
    }
    // Otherwise, assign the connection to a slot
    else
    {
        modbus_connection_state[i] = 1u;
        xtcp_set_connection_appstate(c_modbus,
                                     conn,
                                     (xtcp_appstate_t) &modbus_connection_state[i]);
    }
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
void modbus_reset(chanend c_modbus)
{
    unsigned int index;

    // flush modbus frame
    for(index = 0u; index < MODBUS_SIZE_TCP_DATA; index++)
    {
        modbus_data[index] = 0u;
    }

    // flush all connections
    for(index = 0u; index < MODBUS_NUM_CONNECTIONS; index++)
    {
        modbus_connection_state[index] = 0u;
    }

    modbus_response_length = 0u;

    xtcp_listen(c_modbus, MODBUS_LISTEN_PORT, XTCP_PROTOCOL_TCP);
}

/** =========================================================================
*  Description
*
*  \param xxx    description of xxx
*  \param yyy    description of yyy
*
**/
void modbus_handle_event(chanend c_modbus, xtcp_connection_t *conn)
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

    if (conn->local_port == MODBUS_LISTEN_PORT) {
        switch (conn->event)
        {
        case XTCP_NEW_CONNECTION:
            modbus_tcp_new_connection(c_modbus, conn);
            break;
        case XTCP_RECV_DATA:
            modbus_tcp_recv(c_modbus, conn);
            break;
        case XTCP_SENT_DATA:
        case XTCP_REQUEST_DATA:
        case XTCP_RESEND_DATA:
            modbus_tcp_send(c_modbus, conn->event);
            break;
        case XTCP_TIMED_OUT:
        case XTCP_ABORTED:
        case XTCP_CLOSED:
            modbus_reset(c_modbus);
            break;
        default: break;
        }
        conn->event = XTCP_ALREADY_HANDLED;
    }
    return;
}

/*=========================================================================*/

