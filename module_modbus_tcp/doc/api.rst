Modbus TCP API
==============

Configuration Defines
---------------------

These defines can be set in a file called ``modbus_conf.h`` within your application source tree.

*MODBUS_ADDRESS_COIL_START*
   Start address for coils
   
*MODBUS_ADDRESS_COIL_END*
   End address for coils
   
*MODBUS_ADDRESS_HOLDING_REGISTER_START*
   Start address for Holding Register

*MODBUS_ADDRESS_HOLDING_REGISTER_END*
   End address for Holding Register
   
*MODBUS_ADDRESS_INPUT_REGISTER_START*
   Start address for Input Register
   
*MODBUS_ADDRESS_INPUT_REGISTER_END*
   End address for Input Register
   
*MODBUS_ADDRESS_DISCRETE_INPUT_START*
   Start address for Discrete Input
   
*MODBUS_ADDRESS_DISCRETE_INPUT_END*
   End address for Discrete Input
   
*MODBUS_QUANTITY_COIL_START*
   Start quantity for coils
   
*MODBUS_QUANTITY_COIL_END*
   End quantity for coils
   
*MODBUS_QUANTITY_HOLDING_REGISTER_START*
   Start quantity for Holding Register
   
*MODBUS_QUANTITY_HOLDING_REGISTER_END*
   End quantity for Holding Register
   
*MODBUS_QUANTITY_INPUT_REGISTER_START*
   Start quantity for Input Register
   
*MODBUS_QUANTITY_INPUT_REGISTER_END*
   End quantity for Input Register
   
*MODBUS_QUANTITY_DISCRETE_INPUT_START*
   Start quantity for Discrete Inputs
   
*MODBUS_QUANTITY_DISCRETE_INPUT_END*
   End quantity for Discrete Inputs
   
*MODBUS_WRITE_QUANTITY_1*
   Write quantity for coils 1
   
*MODBUS_WRITE_QUANTITY_2*
   Write quantity for coils 2

   
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

