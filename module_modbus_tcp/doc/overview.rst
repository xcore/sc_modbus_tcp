Overview
========

Modbus TCP component
--------------------

The Modbus TCP component is used to service Modbus TCP clients by implementing a
TCP server. 

Features
++++++++

Implements following Modbus commands:

  * Read Coils
  * Write Single Coil
  * Read Input Register
  * Read Holding Registers
  * Write Single Register
  * Read Discrete Inputs
  
Memory requirements
+++++++++++++++++++

+------------------+---------------+
| Resource         | Usage         |
+==================+===============+
| Stack            |    bytes      |
+------------------+---------------+
| Program          |      bytes    |
+------------------+---------------+

Resource requirements
+++++++++++++++++++++

+---------------+-------+
| Resource      | Usage |
+===============+=======+
| Channels      |       |
+---------------+-------+
| Timers        |       |
+---------------+-------+
| Clocks        |       |
+---------------+-------+
| Logical Cores |       |
+---------------+-------+

Performance
+++++++++++

The achievable effective bandwidth varies according to the available XCore MIPS.

