# COBS-Algorithm
Consistent Overhead Byte Stuffing (COBS)

Based on https://github.com/bakercp/PacketSerial with added functionality for providing a header and trailer. This makes it much more convenient to add your own reliable protocol and checksums.

Excerpt from http://www.stuartcheshire.org/papers/cobsforton.pdf

Byte stuffing is a process that encodes a sequence of data bytes that may contain ‘illegal’ or ‘reserved’ values, using a potentially longer sequence that contains no occurrences of these values. The extra length is referred to here as the overhead of the encoding. To date, byte stuffing algorithms, such as those used by SLIP, PPP and AX.25, have been designed to incur low average overhead, but little effort has been made to minimize their worstcase overhead. However, there are some increasingly popular network devices whose performance is determined more by the worst case than by the average case. <b>Consistent Overhead Byte Stuffing (COBS) tightly bounds the worst-case overhead. It guarantees in the worst case to add no more than one byte in 254 to any packet. For large packets this means that their encoded size is no more than 100.4% of their pre-encoding size. </b>
