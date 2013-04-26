Modbus TCP Demo
===============

:scope: Example
:description: Demonstrates Modbus slave with GPIO slice card
:keywords: ModbusTCP
:boards: XA-SK-E100, XA-SK-GPIO

Demo Overview
=============

This simple demonstration of xTIMEcomposer Studio functionality that uses the XA-SK-E100 and XA-SK-GPIO Slice Cards together with the xSOFTip module_modbus_tcp to demonstrate how the module is used to receive commands from a Modbus Master over TCP and service them to:

- Turn GPIO Slice Card LEDS on and off
- Read the room temperature via the on-board ADC
- Display GPIO Slice Card button presses

Software Requirements
=====================

A Modbus Master application running on the host. For example, SimplyModbus on a PC (http://www.simplymodbus.ca/TCPclient.htm).

Required Repositories
=====================

- sc_modbus_tcp git://github.com/xcore/sc_modbus_tcp.git
- sc_i2c git://github.com/xcore/sc_i2c.git
- xcommon git://github.com/xcore/xcommon.git

Support
=======

Issues may be submitted via the Issues tab in this github repo. Response to any issues submitted as at the discretion of the manitainer for this line.
