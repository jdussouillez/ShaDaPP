#include <cstdlib>
#include <iostream>

#include <shadapp/Logger.h>
#include <shadapp/data/Compression.h>
#include <shadapp/data/Serializer.h>

namespace shadapp {

    namespace data {

        uint32_t LZ4::compress(std::vector<uint8_t>& bytes) {
            int size = bytes.size();
            uint32_t maxCompressedSize = LZ4_COMPRESSBOUND(size + 4); // There's 4 more bytes in the compressed bytes (the size of compressed AND uncompressed msg)
            std::vector<uint8_t> compressedBytes(maxCompressedSize);
            uint32_t compressedSize = LZ4_compress_limitedOutput(
                    reinterpret_cast<const char*> (&bytes[0]),
                    reinterpret_cast<char*> (&compressedBytes[0]),
                    size,
                    maxCompressedSize);
            compressedBytes.resize(compressedSize);
            bytes = compressedBytes;
            return compressedSize;
        }

        uint32_t LZ4::decompress(std::vector<uint8_t>& bytes, uint32_t maxDecompressedSize) {
            std::vector<uint8_t> decompressedBytes(maxDecompressedSize);
            uint32_t uncompressedSize = LZ4_decompress_safe(
                    reinterpret_cast<const char*> (&bytes[0]),
                    reinterpret_cast<char*> (&decompressedBytes[0]),
                    bytes.size(),
                    maxDecompressedSize);
            bytes = decompressedBytes;
            return uncompressedSize;
        }

        void MsgCompresser::compress(std::vector<uint8_t>& bytes) {
            std::vector<uint8_t> compressedMsgBytes;

            // Compress the data
            std::vector<uint8_t> data(bytes.begin() + 8, bytes.end());
            uint32_t compressedDataLength = LZ4::compress(data);
            // Copy the msg's header. 4 bytes, not compressed
            compressedMsgBytes.insert(compressedMsgBytes.end(), bytes.begin(), bytes.begin() + 4);

            // Insert the length of the compressed msg (12 = size Header + Size compressed + size uncompressed). 4 bytes, not compressed
            Serializer::serializeInt32(compressedMsgBytes, 12 + compressedDataLength);

            // Copy the length of the uncompressed msg. 4 bytes, not compressed
            compressedMsgBytes.insert(compressedMsgBytes.end(), bytes.begin() + 4, bytes.begin() + 8);

            // Insert the compressed data
            compressedMsgBytes.insert(compressedMsgBytes.end(), data.begin(), data.end());

            bytes = compressedMsgBytes;
        }

        void MsgCompresser::decompress(std::vector<uint8_t>& bytes) {
            // Here the header is consumed by the super class AbstractMessage (4 bytes)
            std::vector<uint8_t> decompressedMsgBytes;
            // The bytes 4 -> 8 are the size of the compressed msg. Not used here (used when reading the socket to receive the msg)            
            // Get the length of the decompressed msg
            std::vector<uint8_t> decompressedMsgLengthBytes(bytes.begin(), bytes.begin() + 4);
            uint32_t notCompressedMsgLength = Serializer::deserializeInt32(decompressedMsgLengthBytes, true);
            // Decompress the data
            std::vector<uint8_t> data(bytes.begin() + 4, bytes.end());
            uint32_t maxDecompressedDataSize = notCompressedMsgLength - 4;
            LZ4::decompress(data, maxDecompressedDataSize);
            // Insert the decompressed data
            decompressedMsgBytes.insert(decompressedMsgBytes.end(), data.begin(), data.end());
            bytes = decompressedMsgBytes;
            
        }
    }
}
