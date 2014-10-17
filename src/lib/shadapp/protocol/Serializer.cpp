#include <shadapp/protocol/Serializer.h>

namespace shadapp {

    namespace protocol {

        unsigned char* Serializer::serializeInt32(unsigned char* dest, unsigned int offset, uint32_t value, unsigned int* size) {
            unsigned int shift = 24, mask = 0xFF000000;
            for (unsigned int i = offset; i < offset + sizeof (uint32_t); i++) {
                dest[i] = (value & mask) >> shift;
                shift -= 8;
                mask >>= 8;
            }
            *size += sizeof (uint32_t);
            return dest;
        }

        unsigned char* Serializer::serializeInt64(unsigned char* dest, unsigned int offset, uint64_t value, unsigned int* size) {
            unsigned int shift = 56;
            uint64_t mask = 0xFF00000000000000;
            for (unsigned int i = offset; i < offset + sizeof (uint64_t); i++) {
                dest[i] = (value & mask) >> shift;
                shift -= 8;
                mask >>= 8;
            }
            *size += sizeof (uint64_t);
            return dest;
        }

        unsigned char* Serializer::serializeString(unsigned char* dest, unsigned int offset, std::string str, unsigned int* size) {
            int length = str.length();
            for (int i = 0; i < length; i++) {
                dest[offset + i] = str.at(i);
            }
            *size += length;
            return dest;
        }

        uint32_t Serializer::deserializeInt32(unsigned char* bytes, unsigned int* startIndex) {
            uint32_t integer(0);
            unsigned int shift = 24;
            for (unsigned int i = *startIndex; i < *startIndex + sizeof(uint32_t); i++) {
                integer |= bytes[i] << shift;
                shift -= 8;
            }
            *startIndex += sizeof(uint32_t);
            return integer;
        }

        uint64_t Serializer::deserializeInt64(unsigned char* bytes, unsigned int* startIndex) {
            uint64_t integer(0);
            unsigned int shift = 56;
            for (unsigned int i = *startIndex; i < *startIndex + sizeof(uint64_t); i++) {
                integer |= bytes[i] << shift;
                shift -= 8;
            }
            *startIndex += sizeof(uint64_t);
            return integer;
        }

        std::string Serializer::deserializeString(unsigned char* bytes, unsigned int* startIndex, uint32_t length) {
            std::string str((const char*) (bytes + *startIndex), length);
            *startIndex += length;
            return str;
        }
    }
}
