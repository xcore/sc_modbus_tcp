Modbus TCP Server (Slave) Component
===================================

:scope: Early Development
:description: Modbus TCP Server (Slave) component
:keywords: ModbusTCP
:boards: XA-SK-E100

The Modbus TCP component has an in-built TCP server which can receive Modbus TCP commands from a TCP client (Modbus Master). Based on the commands issued by the Modbus Master, the Modbus TCP component requests appropriate data from the user application. It then sends this data back to the Modbus Master (TCP client) and waits for next command. Master functionality is not supported. 

Key Features
------------

Implements the following Modbus commands received over TCP:

   - Read Coils
   - Write Single Coil
   - Read Input Register
   - Read Holding Registers
   - Write Single Register
   - Read Discrete Inputs

Evaluation
----------

Due to the wide range of Modbus master application software on the market and the normally propriatory nature of such software a stand alone demonstration of this component is not available directly in xSOFTip. Instructions for conducting a basic evaluation of this component making use of a free but functionally restricted demo version of the SimplyModbus master software (from http://www.simplymodbus.ca) can be obtained by contacting your XMOS sales representative.

