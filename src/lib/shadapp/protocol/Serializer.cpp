#include <shadapp/protocol/Serializer.h>

namespace shadapp {

    namespace protocol {

        unsigned char* Serializer::serializeInt32(unsigned char* dest, unsigned int offset, uint32_t value, unsigned int* size) {
            unsigned int shift = 24; // 32 - 8
            for (unsigned int i = offset + ((sizeof(int) / 8) - 1); i >= offset; i--) {
                dest[i] = (value >> shift) & 0xFF;
                shift -= 8;
            }
            *size += sizeof(int);
            return dest;
        }

        unsigned char* Serializer::serializeInt64(unsigned char* dest, unsigned int offset, uint64_t value, unsigned int* size) {
            unsigned int shift = 56; // 64 - 8;
            for (unsigned int i = offset + ((sizeof(int) / 8) - 1); i >= offset; i--) {
                dest[i] = (value >> shift) & 0xFF;
                shift -= 8;
            }
            *size += sizeof(int);
            return dest;
        }

        uint32_t Serializer::deserializeInt32(unsigned char* bytes, unsigned int startIndex) {
            uint32_t integer(0);
            unsigned int shift = 24; // 32 - 8
            for (unsigned int i = startIndex; i < startIndex + sizeof(uint32_t); i++) {
                integer |= ((uint32_t) bytes[i]) << shift;
                shift -= 8;
            }
            return integer;
        }

        uint64_t Serializer::deserializeInt64(unsigned char* bytes, unsigned int startIndex) {
            uint64_t integer(0);
            unsigned int shift = 56; // 64 - 8
            for (unsigned int i = startIndex; i < startIndex + sizeof(uint64_t); i++) {
                integer |= ((uint64_t) bytes[i]) << shift;
                shift -= 8;
            }
            return integer;
        }
    }
}
