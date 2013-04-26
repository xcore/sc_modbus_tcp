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

The Modbus TCP slave component accepts data (usually received by the host from TCP layers) and processes them for Modbus commands. If the received TCP data is a Modbus command, the component requests data from/to the user application. The request (Modbus Command) is usually to read/write values from/to registers, coils, etc... After this, the component forms the required Modbus response and waits for next Modbus command.

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
