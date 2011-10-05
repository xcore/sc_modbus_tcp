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
The copyrights, all other intellectual and industrial property rights are
retained by XMOS and/or its licensors.
Terms and conditions covering the use of this code can be found in the Xmos
End User License Agreement.

Copyright XMOS Ltd 2011

In the case where this code is a modification of existing code under a
separate license, the separate license terms are shown below. The
modifications to the code are still covered by the copyright notice above.
==============================================================================*/

/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
//#include <platform.h>
//#include <xs1.h>
//#include <print.h>

#include "system.h"
#include "modbus_tcp.h"
#include "xtcp_client.h"

/*------------------------------------------------------------------------------
Ports and Clocks
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Manifest constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Typedefs
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Global Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Static Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Prototypes
------------------------------------------------------------------------------*/
static void modbus(chanend c_user, chanend c_xtcp);

/*------------------------------------------------------------------------------
Implementation
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
------------------------------------------------------------------------------*/
static void modbus(chanend c_user, chanend c_xtcp)
{
   xtcp_connection_t conn;

   mbtcp_init();

   // Listen on port
   xtcp_listen(c_xtcp, PORT_LISTEN, XTCP_PROTOCOL_TCP);

   // Loop forever processing TCP events
   while(1)
   {
      select
      {
      case xtcp_event(c_xtcp, conn):
         {
            mbtcp_handle_event(c_xtcp, conn, c_user);
            break;
         }
      } // select
   } // while(1)
}

/*------------------------------------------------------------------------------
Name:
Purpose:
Input:
Output:
Notes:
------------------------------------------------------------------------------*/
void xmodbus_tcp(chanend c_user, chanend c_xtcp)
{
   modbus(c_user, c_xtcp);
}

/*============================================================================*/