Introduction
============

This document is the specification for the Modbus TCP Server library using XMOS. 

Below is a data flow diagram. The TCP interface recieves requests from Modbus clients and passes them to the Modbus library. The library checks received data and requests memory interface for the request. This data is then put in a response packet (to which the application has access) and returned back to client by the TCP interface. 

.. figure:: images/modbus_data_flow.png
