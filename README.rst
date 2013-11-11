Modbus TCP Repository
.....................

:Latest release: 1.0.1rc0
:Maintainer: XMOS
:Description: Modbus TCP (Slave) component


Firmware overview
=================

This is a Modbus TCP Server component (A Modbus Slave). Modbus protocol specification and messaging implementation guide can be found here:

http://www.modbus.org/specs.php

The Modbus TCP Server component accepts data from the TCP layers and processes them for Modbus commands. If the received TCP data is a Modbus command, the component requests data from/to the user application. The request (Modbus Command) is usually to read/write values from/to registers, coils, etc... After this, the component forms the required Modbus response, sends it back to the TCP client (Modbus Master) and waits for next Modbus command.

Known issues
============

none

Support
=======

Issues may be submitted via the Issues tab in this github repository. Response to any issues submitted as at the discretion of the maintainer for this line.

Required software (dependencies)
================================

  * sc_xtcp (https://github.com/xcore/sc_xtcp.git)
  * sc_ethernet (git@github.com:xcore/sc_ethernet.git)
  * sc_i2c (https://github.com/xcore/sc_i2c.git)
  * sc_slicekit_support (origin)
  * sc_otp (git@github.com:xcore/sc_otp)
  * sc_util (git@github.com:xcore/sc_util)

