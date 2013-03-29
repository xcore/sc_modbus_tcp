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
#include "mbtcp.h"
#include "mb_parse.h"
#include <string.h>
#include <print.h>

/*---------------------------------------------------------------------------
 constants
 ---------------------------------------------------------------------------*/
#define NUM_HTTPD_CONNECTIONS       10
#define MODBUS_TCP_LISTEN_PORT      502
#define MODBUS_TCP_DATA_SIZE        260

/*---------------------------------------------------------------------------
 ports and clocks
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 typedefs
 ---------------------------------------------------------------------------*/
// Structure to hold HTTP state
typedef struct httpd_state_t
{
  int active;    //< Whether this state structure is being used for a connection
  int conn_id;   //< The connection id
  char *dptr;    //< Pointer to the remaining data to send
  int dlen;      //< The length of remaining data to send
  char *prev_dptr; //< Pointer to the previously sent item of data
} httpd_state_t;

/*---------------------------------------------------------------------------
 global variables
 ---------------------------------------------------------------------------*/
httpd_state_t connection_states[NUM_HTTPD_CONNECTIONS];

/*---------------------------------------------------------------------------
 static variables
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 static prototypes
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 receive http data
 ---------------------------------------------------------------------------*/
void httpd_recv(chanend c_tcp_svr, chanend c_modbus, xtcp_connection_t *conn)
{
  struct httpd_state_t *hs = (struct httpd_state_t *) conn->appstate;
  char data[MODBUS_TCP_DATA_SIZE];
  int len;

  // Receive the data from the TCP stack
  len = xtcp_recv(c_tcp_svr, data);

  // If we already have data to send, return
  if (hs == NULL || hs->dptr != NULL)
  {
    return;
  }

  // Otherwise we have data, so parse it
  hs->dlen = parse_modbus_request(c_modbus, len, &data[0]);

  // If we are required to send data
  if (hs->dlen != 0)
  {
    // Initate a send request with the TCP stack.
    // It will then reply with event XTCP_REQUEST_DATA
    // when it's ready to send
    hs->dptr = &data[0];
    xtcp_init_send(c_tcp_svr, conn);
  }
}

/*---------------------------------------------------------------------------
 send http data
 ---------------------------------------------------------------------------*/
void httpd_send(chanend c_tcp_svr, xtcp_connection_t *conn)
{
  struct httpd_state_t *hs = (struct httpd_state_t *) conn->appstate;

  // Check if we need to resend previous data
  if (conn->event == XTCP_RESEND_DATA)
  {
    xtcp_send(c_tcp_svr, hs->prev_dptr, (hs->dptr - hs->prev_dptr));
    return;
  }

  // Check if we have no data to send
  if (hs->dlen == 0)
  {
    // Terminates the send process
    xtcp_complete_send(c_tcp_svr);
    // Close the connection
    xtcp_close(c_tcp_svr, conn);
  }
  // We need to send some new data
  else
  {
    int len = hs->dlen;

    if (len > conn->mss)
      len = conn->mss;

    xtcp_send(c_tcp_svr, hs->dptr, len);

    hs->prev_dptr = hs->dptr;
    hs->dptr += len;
    hs->dlen -= len;
  }
}

/*---------------------------------------------------------------------------
 init http state
 ---------------------------------------------------------------------------*/
void httpd_init_state(chanend c_tcp_svr, xtcp_connection_t *conn)
{
  int i;

  // Try and find an empty connection slot
  for(i = 0; i < NUM_HTTPD_CONNECTIONS; i++)
  {
    if (!connection_states[i].active)
    {
      break;
    }
  }

  // If no free connection slots were found, abort the connection
  if (i == NUM_HTTPD_CONNECTIONS)
  {
    xtcp_abort(c_tcp_svr, conn);
  }
  // Otherwise, assign the connection to a slot        //
  else
  {
    connection_states[i].active = 1;
    connection_states[i].conn_id = conn->id;
    connection_states[i].dptr = NULL;
    xtcp_set_connection_appstate(c_tcp_svr,
                                 conn,
                                 (xtcp_appstate_t) & connection_states[i]);
  }
}

/*---------------------------------------------------------------------------
 free http state
 ---------------------------------------------------------------------------*/
void httpd_free_state(xtcp_connection_t *conn)
{
  int i;

  for(i = 0; i < NUM_HTTPD_CONNECTIONS; i++)
  {
    if (connection_states[i].conn_id == conn->id)
    {
      connection_states[i].active = 0;
    }
  }
}

/*---------------------------------------------------------------------------
 handle TCP events
 ---------------------------------------------------------------------------*/
void modbus_tcp_handle_event(chanend c_tcp_svr,
                             chanend c_modbus,
                             xtcp_connection_t *conn)
{
  // We have received an event from the TCP stack, so respond
  // appropriately

  // Ignore events that are not directly relevant to http
  switch(conn->event)
  {
    case XTCP_IFUP:
    {
      xtcp_ipconfig_t ipconfig;
      xtcp_get_ipconfig(c_tcp_svr, &ipconfig);

      printstr("IP Address: ");
      printint(ipconfig.ipaddr[0]); printstr(".");
      printint(ipconfig.ipaddr[1]); printstr(".");
      printint(ipconfig.ipaddr[2]); printstr(".");
      printint(ipconfig.ipaddr[3]); printstr("\n");
      return;
    }

    case XTCP_IFDOWN:
    case XTCP_ALREADY_HANDLED: return;

    default: break;
  } // switch(conn->event)

  // Check if the connection is a http connection
  if (conn->local_port == MODBUS_TCP_LISTEN_PORT)
  {
    switch(conn->event)
    {
      case XTCP_NEW_CONNECTION:
      {
        httpd_init_state(c_tcp_svr, conn);
        break;
      }
      case XTCP_RECV_DATA:
      {
        httpd_recv(c_tcp_svr, c_modbus, conn);
        break;
      }
      case XTCP_SENT_DATA:
      case XTCP_REQUEST_DATA:
      case XTCP_RESEND_DATA:
      {
        httpd_send(c_tcp_svr, conn);
        break;
      }
      case XTCP_TIMED_OUT:
      case XTCP_ABORTED:
      case XTCP_CLOSED:
      {
        httpd_free_state(conn);
        break;
      }

      default: break;

    } // switch(conn->event)

    conn->event = XTCP_ALREADY_HANDLED;

  } // if (conn->local_port == MODBUS_TCP_LISTEN_PORT)
}

/*---------------------------------------------------------------------------
 init webserver
 ---------------------------------------------------------------------------*/
void modbus_tcp_init(chanend c_tcp_svr)
{
  int i;
  // Listen on the http port
  xtcp_listen(c_tcp_svr, MODBUS_TCP_LISTEN_PORT, XTCP_PROTOCOL_TCP);

  for(i = 0; i < NUM_HTTPD_CONNECTIONS; i++)
  {
    connection_states[i].active = 0;
    connection_states[i].dptr = NULL;
  }
}

/*==========================================================================*/
