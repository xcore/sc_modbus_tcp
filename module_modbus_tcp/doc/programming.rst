Modbus TCP Programming Guide
============================

This section provides information on how to program applications using the 
Modbus TCP component.

Source code structure
---------------------

.. list-table:: Project structure
  :header-rows: 1
  
  * - Project
    - File
    - Description
  * - module_modbus_tcp
    - ``modbus_tcp_server.h`` 
    - Header file containing the APIs for the Modbus TCP component
  * - 
    - ``modbus_tcp_server.xc``
    - File containing the implementation of the Modbus TCP component
  * - 
    - ``modbus_tcp_cmd.h``
    - Header file containing the Modbus TCP commands
  * - 
    - ``mbparse.*``
    - Parses Modbus commands received and generates response frames
  * - 
    - ``mbtcp.*``
    - The TCP webserver
    
Executing The Project
---------------------

The module by itself cannot be build or executed separately. It must be linked 
to an application which needs Modbus TCP. Once the module is linked to the
application, the application can be built and tested for servicing Modbus 
commands.

The following should be done in order to link the component to the application
project
  #. The module name ``module_modbus_tcp`` (and 
  ``module_ethernet_board_support``) should be added to the list of MODULES in 
  the application project build options.
  #. Now the module is linked to the application and can be directly used
  
Software Requirements
---------------------

The module is built on XDE Tool version 12.2
The module can be used in version 12.2 or any higher version of xTIMEcomposer.

