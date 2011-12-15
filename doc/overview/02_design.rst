Design
======

Modbus TCP library consists of 3 main components:

* The TCP/IP interface
* Modbus frame processing and
* Data interface

The TCP/IP interface is an optional thread based service which uses the TCP module. This can be a part of the user application.

Modbus frame processing unit checks if the incoming frame is valid and builds appropriate responses to send back to the client.

Data interface is closely related to the application specific requirements. User must define the types of data that can be read/written to.

API
---

** modbus_tcp_get_status **

Return type: char modbus_tcp_status - returns status of Modbus TCP frame processing.

+-----------------------------------------------+
|  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |  
+-----------------------------------------------+
| Res | Res | Res | Res | Res | Res |VALID| MB  |  
+-----------------------------------------------+

Param: - 

Description:

Here is the bit field information about the status.

Bit 0: A 'zero' at this position indicates that the TCP data received is not a Modbus frame and hence cannot be processed further. A 'one' indicates a valid Modbus frame.

Bit 1: A 'zero' indicates that the frame has error in Modbus request. A 'one' indicates a valid Modbus frame. The value at this bit field is valid only if a 'one' is read at Bit 0.

Bit 2 - 7: Reserved.

This function must be called from the user application space.

** modbus_tcp_process_frame **

Return type: int response_length - returns length of response frame.

Param:

* char *data[] - Pointer to the TCP data received.
* int length - Length of data received.

Description:

The user must pass the address and length of data received from TCP layer. This function checks if the frame received is a valid Modbus TCP frame. It then calls the memory interface function and modifies the data with response information. It returns the response frame length of Modbus processing. 

** is_modbus_tcp_frame **

Return type: boolean

Param: 

* char *data[] - Pointer to the TCP data received.

Description:

Checks if 'protocol' field of the received TCP frame matches Modbus Protocol code (zero). Return 'true' for a match or 'false' for a mismatch. This is called internally in library. Status must be updated with the return value by this function.

** is_valid_request **

Return type: boolean

Param: 

* char *data[] - Pointer to the TCP data received.

Description:

This is called internally in library. Status must be updated with the return value by this function. Checks if other fields of the received Modbus TCP frame are valid. It checks the Function code, address and data renges, etc... Return 'true' for a valid request or 'false' for an invalid request.

** memory_access **

Return type: boolean - success / failure

Param: 

* int element_index - motor number.
* int function_code - the type of access to be performed.
* char *m_data[] - array where the memory functions would read from / write to, based on function requested.
* int start_address - starting address.
* int length - starting address + length = end address.

Description:

Memory access functions can be taken out of the library and suit the application needs. The function parameters have all the information required to access a particular memory type.

Example
-------

.. figure:: images/example_app.png

