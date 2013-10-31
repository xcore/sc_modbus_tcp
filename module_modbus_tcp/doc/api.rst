.. _modbus_tcp_api:

API
====

Configuration Defines
---------------------

*Ethernet Slot*

The XP-SKC-L16 sliceKIT Core board has four slots with edge connectors: ``SQUARE``, ``CIRCLE``, ``TRIANGLE`` and ``STAR``. The user must indicate which slot, the ``XA-SK-E100`` Ethernet sliceCARD, is connected on the sliceKIT Core Board ``XA-SKC-L16``. It should be of the form (one of the following):

- #define MODBUS_TCP_ETHERNET_SLOT_STAR
- #define MODBUS_TCP_ETHERNET_SLOT_TRIANGLE
- #define MODBUS_TCP_ETHERNET_SLOT_CIRCLE
- #define MODBUS_TCP_ETHERNET_SLOT_SQUARE

If nothing is defined, then the module assumes that the Ethernet sliceCARD is connected to the ``CIRCLE`` slot on the sliceKIT core board.

Server functions
----------------

.. doxygenfunction:: modbus_tcp_server

Modbus commands & Status
------------------------

.. doxygenenum:: modbus_cmd

.. doxygenenum:: modbus_error

