Programming Guide
=================

Using the Module
----------------

To use the module you need to add ``module_modbus_tcp`` to the ``USED_MODULES`` variable in your application Makefile.

``modbus_tcp.h``
   Include this file in your application to call the Modbus TCP library function to parse Modbus TCP commands from a master.
   
Within your application you can also add the following files to process Modbus TCP commands:

``modbus_conf.h``
   A configuration file to specify the address and quantity limits of Coils/Registers. It must also have the function call-backs point to the correct functions.
   
``modbus_cb_functions.h``
   A header file that can be included in the ``modbus_conf.h`` for function call-back definitions. The Modbus call-back functions must be prototyped here as described in the API section. The implmentation of these functions can be in the user application space.

Integrating the webserver into your code
----------------------------------------

To use the webserver you must have an instance of an XTCP server running on your system. You can then write a function that implements a task that handles tcp events. This may do other functions/handle other tcp traffic as well as implementing the webserver. Here is a rough template of how the code should look::

   void xhttpd(chanend c_tcp_svr)
   {
     xtcp_connection_t conn;

     // Initiate the HTTP state

     while(1)
     {
       select
       {
         // Listen to XTCP events
         case xtcp_event(c_tcp_svr, conn):
         {
           // Handle XTCP Events here
           
           // When some data is received over the TCP, call the Modbus library function to parse the command by passing the data as a parameter.
           
           // The library function will exit, returning the length of Modbus response. The response is in the same data which was passed as a parameter.
           
           break;
         }
       }
     }
   }

