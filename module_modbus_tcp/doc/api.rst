Modbus TCP API
==============

Configuration Defines
---------------------

These defines can be set in a file called ``modbus_conf.h`` within your application source tree.

.. doxygendefine:: MODBUS_ADDRESS_COIL_START
.. doxygendefine:: MODBUS_ADDRESS_COIL_END
.. doxygendefine:: MODBUS_ADDRESS_HOLDING_REGISTER_START
.. doxygendefine:: MODBUS_ADDRESS_HOLDING_REGISTER_END
.. doxygendefine:: MODBUS_ADDRESS_INPUT_REGISTER_START
.. doxygendefine:: MODBUS_ADDRESS_INPUT_REGISTER_END
.. doxygendefine:: MODBUS_ADDRESS_DISCRETE_INPUT_START
.. doxygendefine:: MODBUS_ADDRESS_DISCRETE_INPUT_END
.. doxygendefine:: MODBUS_QUANTITY_COIL_START
.. doxygendefine:: MODBUS_QUANTITY_COIL_END
.. doxygendefine:: MODBUS_QUANTITY_HOLDING_REGISTER_START
.. doxygendefine:: MODBUS_QUANTITY_HOLDING_REGISTER_END
.. doxygendefine:: MODBUS_QUANTITY_INPUT_REGISTER_START
.. doxygendefine:: MODBUS_QUANTITY_INPUT_REGISTER_END
.. doxygendefine:: MODBUS_QUANTITY_DISCRETE_INPUT_START
.. doxygendefine:: MODBUS_QUANTITY_DISCRETE_INPUT_END
.. doxygendefine:: MODBUS_WRITE_QUANTITY_1
.. doxygendefine:: MODBUS_WRITE_QUANTITY_2

Call-back function defines
--------------------------

.. c:macro:: MB_READ_COILS

This define can be set to a function that will be run after a valid ``Read Coil`` command from the Modbus TCP master. This function must read the addressed coil and return its value. The function must be defined as::

   unsigned short read_coils(unsigned short address)
   
.. c:macro:: MB_READ_DISCRETE_INPUTS

This define can be set to a function that will be run after a valid ``Read Discrete Inputs`` command from the Modbus TCP master. This function must read the addressed discrete inputs and return its value. The function must be defined as::

   unsigned short read_discrete_inputs(unsigned short address)
   
.. c:macro:: MB_READ_HOLDING_REGISTERS

This define can be set to a function that will be run after a valid ``Read Holding Registers`` command from the Modbus TCP master. This function must read the addressed Holding Register and return its value. The function must be defined as::

   unsigned short read_holding_registers(unsigned short address)
   
.. c:macro:: MB_READ_INPUT_REGISTERS

This define can be set to a function that will be run after a valid ``Read Input Registers`` command from the Modbus TCP master. This function must read the addressed Input Register and return its value. The function must be defined as::

   unsigned short read_input_registers(unsigned short address)
   
.. c:macro:: MB_WRITE_SINGLE_COIL

This define can be set to a function that will be run after a valid ``Write Single Coil`` command from the Modbus TCP master. This function must write to the addressed coil and return if the write was successful (1) or a failure (0). The function must be defined as::

   unsigned short write_single_coil(unsigned short address, unsigned short value)
   
.. c:macro:: MB_WRITE_SINGLE_REGISTER

This define can be set to a function that will be run after a valid ``Write Single Register`` command from the Modbus TCP master. This function must write to the addressed Register and return if the write was successful (1) or a failure (0). The function must be defined as::

   unsigned short write_single_register(unsigned short address, unsigned short value)
   
Modbus TCP parsing functions
-----------------------------

.. doxygenfunction:: modbus_tcp_parse_request

