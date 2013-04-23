Modbus TCP Demo Quick start Guide
=================================

This simple demonstration of xTIMEcomposer Studio functionality that uses the ``XA-SK-E100`` and ``XA-SK-GPIO`` Slice Cards together with the xSOFTip ``module_modbus_tcp``, ``module_xtcp`` and ``module_ethernet`` to demonstrate how the module is used to receive commands from a Modbus Master over TCP and service them to modify/read GPIO states on the GPIO slice card.

Hardware Setup
++++++++++++++

The XP-SKC-L2 Slicekit Core board has four slots with edge connectors: ``SQUARE``, ``CIRCLE``, ``TRIANGLE`` and ``STAR``.

To setup up the system:

#. Connect XA-SK-E100 Slice Card to the XP-SKC-L2 Slicekit Core board using the connector marked with the ``CIRCLE``.
#. Connect XA-SK-GPIO Slice Card to the XP-SKC-L2 Slicekit Core board using the connector marked with the ``SQUARE``.
#. Connect the XTAG Adapter to Slicekit Core board, and connect XTAG-2 to the adapter.
#. Connect the XTAG-2 to host PC. Note that the USB cable is not provided with the Slicekit starter kit.
#. Connect an Ethernet cable between the XA-SK-E100 Slice Card and host PC.
#. Switch on the power supply to the Slicekit Core board.

.. figure:: images/hardware_setup.jpg
   :align: center

   Hardware Setup for Modbus TCP Demo

Import and Build the Application
++++++++++++++++++++++++++++++++

#. Open xTIMEcomposer and check that it is operating in online mode. Open the edit perspective (Window->Open Perspective->XMOS Edit).
#. Locate the ``Modbus TCP Demo`` item in the xSOFTip pane on the bottom left of the window and drag it into the Project Explorer window in the xTIMEcomposer. This will also cause the modules on which this application depends to be imported as well.
#. Click on the ``app_modbus_tcp`` item in the Project Explorer pane then click on the build icon (hammer) in xTIMEcomposer. Check the console window to verify that the application has built successfully.

For help in using xTIMEcomposer, try the xTIMEcomposer tutorial, which you can find by selecting Help->Tutorials from the xTIMEcomposer menu.

Note that the Developer Column in the xTIMEcomposer on the right hand side of your screen provides information on the xSOFTip components you are using. Select the module_modbus_tcp component in the Project Explorer, and you will see its description together with API documentation. Having done this, click the `back` icon until you return to this quickstart guide within the Developer Column.

Run the Application
+++++++++++++++++++

Now that the application has been compiled, the next step is to run it on the Slicekit Core Board using the tools to load the application over JTAG (via the XTAG2 and XTAG Adapter card) into the xCORE multicore microcontroller.

- Select the file ``app_modbus_tcp.xc`` in the ``app_modbus_tcp`` project from the Project Explorer.
- Click on the ``Run`` icon (the white arrow in the green circle).
- At the ``Select Device`` dialog select ``XMOS XTAG-2 connect to L1[0..1]`` and click ``OK``.
- The console output should print an IP address.

   Example: IP address: 169.254.231.27

- On your PC, open the SimplyModbus Client from Programs -> SimplyModbus and adjust to following settings:

   - mode = TCP
   - IP Address = user ip address provided in main.xc
   - Port = 502 (Modbus Listening Port)

- Alternatively, load the read_coil configuration file from $\\app_modbus_tcp\\simplymodbus_config\\ and click CONNECT.

.. figure:: images/init.png
   :align: center

   SimplyModbus Configuration

- Send a command as 'Read Coil' (load read_coil config from $\\app_modbus_tcp\\simplymodbus_config\\). This will read the status of GPIO LED on XA-SK-GPIO slice card. To read the status of the other three LEDs, change the ``First Coil`` value in the SimplyModbus application.

   ============ ===================
   First coil    LED on Slice card
   ============ ===================
   1             0
   2             1
   3             2
   4             3
   ============ ===================

.. figure:: images/read_coil.png
   :align: center

   Read Coil

- Send a 'Read Input Register' (load config from $\\app_modbus_tcp\\simplymodbus_config\\) command to read the current room temperature as recorded by the sensor present on the XA-SK-GPIO slice card.

.. figure:: images/read_ip_reg.png
   :align: center
   
   Read Input Register

- Send a 'Read Discrete Input' (load config from $\\app_modbus_tcp\\simplymodbus_config\\) command to read button status on the XA-SK-GPIO slice card.

.. figure:: images/read_dis_ip.png
   :align: center
   
   Read Discrete Input
      
- You can write using the WRITE interface. This can be brought up by clicking the WRITE button in simplyModbus. Load the write_coil config in WRITE window. Sending the command will toggle the requested LED.

.. figure:: images/write_coil.png
   :align: center

   Write coil in SimplyModbus

User can change appropriate values of address and data based on your application. Any unsupported Function code or data range will result in exception response. For example:

.. figure:: images/7.png
   :align: center

   Illegal Function Code exception response example

.. figure:: images/8.png
   :align: center

   Illegal Data Value exception response example

Examine the application code
++++++++++++++++++++++++++++

**File Structure**

.. list-table:: Project structure
  :header-rows: 1
  
  * - Project
    - File
    - Description
  * - module_modbus_tcp
    - ``modbus_tcp.h`` 
    - Header file containing the APIs for the Modbus TCP component
  * - app_modbus_tcp
    - ``app_modbus_tcp.xc``
    - File containing main() function for the application. Also contains Modbus call-back functions implementation.
  * - 
    - ``modbus_cb_functions.h``
    - Header file containing prototypes for Modbus call-back functions.
  * - 
    - ``modbus_conf.h``
    - Modbus configuration file. Defines addresses and quantities for coils / registers and function call-backs.
  * - 
    - ``webserver\xhttpd.h``
    - The TCP webserver and Button listener.
  * - 
    - ``webserver\httpd.h``
    - The TCP event handler. Calls Modbus library function when a TCP frame is received and sends responses.

