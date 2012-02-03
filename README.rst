Modbus TCP Component
....................

:Stable release:  unreleased

:Status:  example

:Maintainer:  Vinith Mundhra (github: vinithmundhra)

:Description:  Modbus TCP Component - Server


Firmware Overview
=================

This is a Modbus TCP Server library. Modbus protocol specification and messaging implementation guide can be found here:

http://www.modbus.org/specs.php

A Modbus client (e.g. on a PC) can send 'requests' to the Modbus Server running in an Xcore via TCP. The TCP layer 
accepts this data from the client and passes it over to the modbus_event_handler. 

This frame is then processed by modbus_pkt_processing and a response is generated which is carried back to the client 
via TCP.

Demo Application
================

A demo application using the SImplyModbus client side software and an XC-2 Ethernet development board is included.

Documentation
=============

See http://github.xcore.com/sc_modbus_tcp/ for design documentation and demo quick start guide.

To Do
=====

* A single thread variant that can process Modbus requests in parallel to TCP communications.


Required Repositories
================

* sc_ethernet git://github.com/xcore/sc_ethernet.git
* sc_xtcp git://github.com/xcore/sc_xtcp.git
* xcommon git\@github.com:xcore/xcommon.git

Support
=======

Issues may be submitted via the Issues tab in this github repo. Response to any issues submitted as at the discretion of the manitainer for this line.
