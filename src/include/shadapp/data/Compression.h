#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <cstdint>
#include <vector>

#include "lz4.h"

namespace shadapp {

    namespace data {

        class LZ4 {
        public:
            static uint32_t compress(std::vector<uint8_t>& bytes);
            static uint32_t decompress(std::vector<uint8_t>& bytes, uint32_t maxDecompressedSize);
        };
        
        class MsgCompresser {
        public:
            static void compress(std::vector<uint8_t>& bytes);
            static void decompress(std::vector<uint8_t>& bytes);
        };
    }
}

#endif	// COMPRESSION_H
