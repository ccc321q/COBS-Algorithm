# COBS-Algorithm
Consistent Overhead Byte Stuffing (COBS)

It guarantees in the worst case to add no more than one byte in 254 to any packet. For large packets this means that their encoded size is no more than 100.4% of their pre-encoding size. 

Based on https://github.com/bakercp/PacketSerial with added functionality for providing a header and trailer. This makes it much more convenient to add your own reliable protocol and checksums.

Check http://www.stuartcheshire.org/papers/cobsforton.pdf for algorithm details.
