Modbus TCP Component
....................

:Stable release:  unreleased
:Maintainer:  vinithmundhra
:Description:  Modbus TCP Component

:Status:  example

Key Features
============

Implements following Modbus commands:

- Read Coils
- Write Single Coil
- Read Input Register
- Read Holding Registers
- Write Single Register
- Read Discrete Inputs

Firmware Overview
=================

This is a Modbus TCP component. Modbus protocol specification and messaging implementation guide can be found here:

http://www.modbus.org/specs.php

A Modbus client (e.g. on a PC - Simply Modbus) can send 'requests' to the Modbus Server running in an XCore via TCP. The TCP layer accepts this data from the client and passes it over to the modbus_tcp_handle_event.

This frame is then processed and a response is requested from the application over the Modbus channel. The response generated is carried back to the client via TCP.

Known Issues
============

none

Required Repositories
=====================

- sc_ethernet git://github.com/xcore/sc_ethernet.git
- sc_xtcp git://github.com/xcore/sc_xtcp.git

Support
=======

Issues may be submitted via the Issues tab in this github repo. Response to any issues submitted as at the discretion of the manitainer for this line.
