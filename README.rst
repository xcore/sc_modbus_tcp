Modbus TCP Component
....................

:Stable release:  unreleased

:Status:  draft

:Maintainer:  Vinith Mundhra (github: vinithmundhra)

:Description:  Modbus TCP Component - Server


Overview
========

This is a Modbus TCP Server library. Modbus protocol specification and messaging implementation guide can be found here: http://www.modbus.org/specs.php

A Modbus client can send 'requests' to a Modbus Server via TCP. These requests may be to read / write a certain part of memory. The TCP layer carries this data from the client and passes it over to the modbus_event_handler. This frame is then processed by modbus_pkt_processing and a response is generated. This response is carried back to the client via TCP.

To Do
=====

* A single thread variant that can process Modbus requests in parallel to TCP communications.

Firmware Overview
=================

* module_modbus_tcp_server\src\modbus_tcpip_if.*

Handles TCP events and calls appropriate Modbus commands to process requests.

* module_modbus_tcp_server\src\modbus_pkt_processing.*

Actually, processes the request and generates (positive or negative) response.

Known Issues
============

Please report issues via the Issues tab in this github repo.

Required Repositories
================

* sc_ethernet git://github.com/xcore/sc_ethernet.git
* sc_xtcp git://github.com/xcore/sc_xtcp.git
* xcommon git\@github.com:xcore/xcommon.git

Support
=======

Issues may be submitted via the Issues tab in this github repo. Response to any issues submitted as at the discretion of the manitainer for this line.
