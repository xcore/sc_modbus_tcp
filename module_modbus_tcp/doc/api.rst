Modbus TCP API
==============

Configuration Defines
---------------------

These defines may be set in a file called ``modbus_conf.h`` within your application source tree. If nothing is set, then the module assumes some default values.


.. list-table:: Defines for Addresses and Quantities
  :header-rows: 1
  
  * - Define
    - Description
    - Min
    - Max
    - Default
  * - MODBUS_ADDRESS_COIL_START
    - Start address for coils
    - 0
    - 65535
    - 0
  * - MODBUS_ADDRESS_COIL_END
    - End address for coils
    - 0
    - 65535
    - 65535
  * - MODBUS_ADDRESS_HOLDING_REGISTER_START
    - Start address for Holding Register
    - 0
    - 65535
    - 0
  * - MODBUS_ADDRESS_HOLDING_REGISTER_END
    - End address for Holding Register
    - 0
    - 65535
    - 65535
  * - MODBUS_ADDRESS_INPUT_REGISTER_START
    - Start address for Input Register
    - 0
    - 65535
    - 0
  * - MODBUS_ADDRESS_INPUT_REGISTER_END
    - End address for Input Register
    - 0
    - 65535
    - 65535
  * - MODBUS_ADDRESS_DISCRETE_INPUT_START
    - Start address for Discrete Input
    - 0
    - 65535
    - 0
  * - MODBUS_ADDRESS_DISCRETE_INPUT_END
    - End address for Discrete Input
    - 0
    - 65535
    - 65535
  * - MODBUS_QUANTITY_COIL_START
    - Start quantity for coils
    - 1
    - 2000
    - 1
  * - MODBUS_QUANTITY_COIL_END
    - End quantity for coils
    - 1
    - 2000
    - 2000
  * - MODBUS_QUANTITY_HOLDING_REGISTER_START
    - Start quantity for Holding Register
    - 1
    - 125
    - 1
  * - MODBUS_QUANTITY_HOLDING_REGISTER_END
    - End quantity for Holding Register
    - 1
    - 125
    - 125
  * - MODBUS_QUANTITY_INPUT_REGISTER_START
    - Start quantity for Input Register
    - 1
    - 125
    - 1
  * - MODBUS_QUANTITY_INPUT_REGISTER_END
    - End quantity for Input Register
    - 1
    - 125
    - 125
  * - MODBUS_QUANTITY_DISCRETE_INPUT_START
    - Start quantity for Discrete Inputs
    - 1
    - 2000
    - 1
  * - MODBUS_QUANTITY_DISCRETE_INPUT_END
    - End quantity for Discrete Inputs
    - 1
    - 2000
    - 2000
  * - MODBUS_WRITE_QUANTITY_1
    - Write quantity for coils 1
    - 0
    - 0
    - 0
  * - MODBUS_WRITE_QUANTITY_2
    - Write quantity for coils 2
    - 65280
    - 65280
    - 65280


    
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

