
#pragma once

#include <stdint.h>

/// \brief A Consistent Overhead Byte Stuffing (COBS) Encoder.
///
/// Consistent Overhead Byte Stuffing (COBS) is an encoding that removes all 0
/// bytes from arbitrary binary data. The encoded data consists only of bytes
/// with values from 0x01 to 0xFF. This is useful for preparing data for
/// transmission over a serial link (RS-232 or RS-485 for example), as the 0
/// byte can be used to unambiguously indicate packet boundaries. COBS also has
/// the advantage of adding very little overhead (at least 1 byte, plus up to an
/// additional byte per 254 bytes of data). For messages smaller than 254 bytes,
/// the overhead is constant.
///
/// \sa http://conferences.sigcomm.org/sigcomm/1997/papers/p062.pdf
/// \sa http://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
/// \sa https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing
/// \sa http://www.jacquesf.com/2011/03/consistent-overhead-byte-stuffing/
class COBS
{
public:
    /// \brief Encode a byte buffer with the COBS encoder.
    /// \param sourceHeader The header buffer to encode.
    /// \param sourceHeaderSize The size of the header buffer to encode.
    /// \param sourceBody The body buffer to encode. This is usally the packet contents.
    /// \param sourceBodySize The size of the body buffer to encode.
    /// \param sourceTrailer The trailer buffer to encode. This is usually the checksum of the header and body.
    /// \param sourceTrailerSize The size of the trailer buffer to encode.
    /// \param destination The target buffer for the encoded bytes.
    /// \returns The number of bytes in the encoded buffer.
    /// \warning destination must have a minimum capacity of
    ///     ((headerSize + sourceSize + trailerSize) + (headerSize + sourceSize + trailerSize) / 254 + 1).
    static uint16_t encode(const uint8_t* sourceHeader,  uint16_t sourceHeaderSize,
                           const uint8_t* sourceBody,    uint16_t sourceBodySize,
                           const uint8_t* sourceTrailer, uint16_t sourceTrailerSize,
                           uint8_t* destination)
    {
        uint16_t read_index  = 0;
        uint16_t write_index = 1;
        uint16_t code_index  = 0;
        uint8_t  code        = 1;

        // Encode sourceHeader
        while(read_index < sourceHeaderSize)
        {
            if(sourceHeader[read_index] == 0)
            {
                destination[code_index] = code;
                code = 1;
                code_index = write_index++;
                read_index++;
            }
            else
            {
                destination[write_index++] = sourceHeader[read_index++];
                code++;

                if(code == 0xFF)
                {
                    destination[code_index] = code;
                    code = 1;
                    code_index = write_index++;
                }
            }
        }

        read_index  = 0;

        // Encode sourceBody
        while(read_index < sourceBodySize)
        {
            if(sourceBody[read_index] == 0)
            {
                destination[code_index] = code;
                code = 1;
                code_index = write_index++;
                read_index++;
            }
            else
            {
                destination[write_index++] = sourceBody[read_index++];
                code++;

                if(code == 0xFF)
                {
                    destination[code_index] = code;
                    code = 1;
                    code_index = write_index++;
                }
            }
        }

        read_index  = 0;

        // Encode sourceTrailer
        while(read_index < sourceTrailerSize)
        {
            if(sourceTrailer[read_index] == 0)
            {
                destination[code_index] = code;
                code = 1;
                code_index = write_index++;
                read_index++;
            }
            else
            {
                destination[write_index++] = sourceTrailer[read_index++];
                code++;

                if(code == 0xFF)
                {
                    destination[code_index] = code;
                    code = 1;
                    code_index = write_index++;
                }
            }
        }

        destination[code_index] = code;

        return write_index;
    }

    /// \brief Decode a COBS-encoded buffer.
    /// \param source The COBS-encoded buffer to decode.
    /// \param size The size of the COBS-encoded buffer.
    /// \param destination The target buffer for the decoded bytes.
    /// \returns The number of bytes in the decoded buffer.
    /// \warning destination must have a minimum capacity of
    ///     size
    static uint16_t decode(const uint8_t* source, uint16_t size, uint8_t* destination)
    {
        uint16_t read_index  = 0;
        uint16_t write_index = 0;
        uint8_t code;
        uint8_t i;

        while(read_index < size)
        {
            code = source[read_index];

            if(read_index + code > size && code != 1)
            {
                return 0;
            }

            read_index++;

            for(i = 1; i < code; i++)
            {
                destination[write_index++] = source[read_index++];
            }

            if(code != 0xFF && read_index != size)
            {
                destination[write_index++] = '\0';
            }
        }

        return write_index;
    }

    /// \brief Get the maximum encoded buffer size needed for a given source size.
    /// \param headerSize The size of the header buffer to be encoded.
    /// \param sourceSize The size of the buffer to be encoded.
    /// \param trailerSize The size of the trailer buffer to be encoded.
    /// \returns the maximum size of the required encoded buffer.
    static uint16_t getEncodedBufferSize(uint16_t headerSize, uint16_t sourceSize, uint16_t trailerSize)
    {
        uint16_t total = headerSize + sourceSize + trailerSize;
        return total + total / 254 + 1;
    }
};
