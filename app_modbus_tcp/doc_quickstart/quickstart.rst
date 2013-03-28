.. _modbus_tcp_server_demo_Quickstart:

Modbus TCP Server Demo Quickstart Guide
=======================================

sc_modbus_tcp demo : Quick Start Guide
--------------------------------------

This simple demonstration of xTIMEcomposer Studio functionality that uses the
XA-SK-E100 Slice Card together with the xSOFTip ``module_modbus_tcp_server`` to
demonstrate how the module is used to service commands from Modbus TCP client.

Hardware Setup
++++++++++++++

The XP-SKC-L2 Slicekit Core board has four slots with edge connectors:
``SQUARE``, ``CIRCLE``, ``TRIANGLE`` and ``STAR``.

To setup up the system:

  #. Connect XA-SK-E100 Slice Card to the XP-SKC-L2 Slicekit Core board using
  the connector marked with the ``CIRCLE``.
  #. Connect the XTAG Adapter to Slicekit Core board, and connect XTAG-2 to the
  adapter.
  #. Connect the XTAG-2 to host PC. Note that the USB cable is not provided with
  the Slicekit starter kit.
  #. Connect an Ethernet cable between the XA-SK-E100 Slice Card and host PC.
  #. Switch on the power supply to the Slicekit Core board.

.. figure:: images/hardware_setup.png
    :align: center

    Hardware Setup for Modbus TCP Server Demo

Import and Build the Application
++++++++++++++++++++++++++++++++

  #. Open xTIMEcomposer and check that it is operating in online mode. Open the
  edit perspective (Window->Open Perspective->XMOS Edit).
  #. Locate the ``'Slicekit Modbus TCP Server Demo'`` item in the xSOFTip pane
  on the bottom left of the window and drag it into the Project Explorer window
  in the xTIMEcomposer. This will also cause the modules on which this
  application depends (in this case, module_xtcp, module_ethernet) to be
  imported as well.
  #. Click on the ``app_modbus_tcp`` item in the Project Explorer pane then
  click on the build icon (hammer) in xTIMEcomposer. Check the console window to
  verify that the application has built successfully.

For help in using xTIMEcomposer, try the xTIMEcomposer tutorial, which you can
find by selecting Help->Tutorials from the xTIMEcomposer menu.

Note that the Developer Column in the xTIMEcomposer on the right hand side of
your screen provides information on the xSOFTip components you are using. Select
the module_lcd component in the Project Explorer, and you will see its
description together with API documentation. Having done this, click the `back`
icon until you return to this quickstart guide within the Developer Column.

Run the Application
+++++++++++++++++++

Now that the application has been compiled, the next step is to run it on the
Slicekit Core Board using the tools to load the application over JTAG (via the
XTAG2 and Xtag Adapter card) into the xCORE multicore microcontroller.

  #. Select the file ``main.xc`` in the ``app_modbus_tcp`` project
  from the Project Explorer.
  #. Click on the ``Run`` icon (the white arrow in the green circle).
  #. At the ``Select Device`` dialog select ``XMOS XTAG-2 connect to L1[0..1]``
  and click ``OK``.
  #. The console output should print an IP address.

    Example: IP address: 169.254.190.106

  #. On your PC, open the SimplyModbus Client from Programs -> SimplyModbus and
  adjust to following settings:

    **mode = TCP**
    **IP Address = user ip address provided in main.xc**
    **Port = 502 (Modbus Listening Port)**

  #. Alternatively, load the read_coil configuration file from
  $\app_modbus_tcp\simplymodbus_config\ and click CONNECT.

  .. figure:: images/1.png
    :align: center

    SimplyModbus Configuration

  #. Send a command as 'Read Coil' (load read_coil config from
  $\app_modbus_tcp\simplymodbus_config\). This will read the status of LED B.
  The status can be seen as SET / CLEARED bits in results column. Please note
  that the status of one LED is copied to all other requested quantities.

  .. figure:: images/2.png
    :align: center

    Read Coil

  #. You can write using the WRITE interface. This can be brought up by clicking
  the WRITE button in simplyModbus.

  .. figure:: images/3.png
    :align: center

    Write interface in SimplyModbus

  #. Load the write_coil config in WRITE window. Sending the command will illuminate LED B.

  .. figure:: images/4.png
    :align: center

    After 'write', read back the status of LED B


User can change appropriate values of address and data based on his application.
Any unsupported Function code or data range will result in exception response.
For example:

.. figure:: images/7.png
  :align: center

  Illegal Function Code exception response example

.. figure:: images/8.png
  :align: center

  Illegal Data Value exception response example

Next Steps
++++++++++

  #. Examine the application code. In xTIMEcomposer navigate to the ``src`` 
  directory under app_modbus_tcp and double click on the ``main.xc`` file within 
  it. The file will open in the central editor window.
  #. Try changing the modbus responses to commands.
