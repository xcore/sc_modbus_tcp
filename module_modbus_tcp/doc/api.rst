Project structure
=================

The component ``sc_modbus_tcp`` includes the module ``module_modbus_tcp``.

module_modbus_tcp
-----------------

Configuration Defines
+++++++++++++++++++++

The configuration defines are listed in ``modbus_config.h``.

**MODBUS_ADDRESS_COIL_START**
  Start address for coils
  
**MODBUS_ADDRESS_COIL_END**
  End address for coils
  
**MODBUS_ADDRESS_HOLDING_REGISTER_START**
  Start address for Holding Register
  
**MODBUS_ADDRESS_HOLDING_REGISTER_END**
  End address for Holding Register
  
**MODBUS_ADDRESS_INPUT_REGISTER_START**
  Start address for Input Register
  
**MODBUS_ADDRESS_INPUT_REGISTER_END**
  End address for Input Register
  
**MODBUS_ADDRESS_DISCRETE_INPUT_START**
  Start address for Discrete Input
  
**MODBUS_ADDRESS_DISCRETE_INPUT_END**
  End address for Discrete Input
  
**MODBUS_QUANTITY_COIL_START**
  Start quantity for coils
  
**MODBUS_QUANTITY_COIL_END**
  End quantity for coils
  
**MODBUS_QUANTITY_HOLDING_REGISTER_START**
  Start quantity for Holding Register
  
**MODBUS_QUANTITY_HOLDING_REGISTER_END**
  Start quantity for Holding Register
  
**MODBUS_QUANTITY_INPUT_REGISTER_START**
  Start quantity for Input Register
  
**MODBUS_QUANTITY_INPUT_REGISTER_END**
  Start quantity for Input Register
  
**MODBUS_QUANTITY_DISCRETE_INPUT_START**
  Start quantity for Discrete Inputs
  
**MODBUS_QUANTITY_DISCRETE_INPUT_END**
  Start quantity for Discrete Inputs
  
**MODBUS_WRITE_QUANTITY_1**
  Write quantity for coils 1
  
**MODBUS_WRITE_QUANTITY_2**
  Write quantity for coils 2

API
+++

The Modbus TCP module functionality is defined in

  - ``modbus_tcp_server.xc``
  - ``modbus_tcp_server.h``
  - ``modbus_tcp_cmd.h``
  
where the following functions can be found:

.. doxygenfunction:: modbus_tcp_server
