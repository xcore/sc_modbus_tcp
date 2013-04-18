Modbus TCP Library Programming Guide
====================================

This software block parses Modbus TCP commands from a Modbus master. It calls-back appropriate application functions to process Modbus TCP commands. Finally, it generates Modbus TCP responses that can be sent to the Modbus master by the application over TCP.

The following Modbus TCP commands are supported:

- Read Coils
- Write Single Coil
- Read Input Register
- Read Holding Registers
- Write Single Register
- Read Discrete Inputs

The memory and resource requirements for this module:

+------------------+---------------+
| Resource         | Usage         |
+==================+===============+
| Stack            |        bytes  |
+------------------+---------------+
| Program          |        bytes  |
+------------------+---------------+
| Channels         |               |
+------------------+---------------+
| Timers           |               |
+------------------+---------------+
| Clocks           |               |
+------------------+---------------+
| Logical Cores    |               |
+------------------+---------------+

.. toctree::

   getting_started
   programming
   api
