#ifndef SERIALIZER_H
#define	SERIALIZER_H

#include <cstdint>
#include <string>

namespace shadapp {

    namespace data {

        class Serializer {
        public:
            static unsigned char* serializeInt32(unsigned char* dest, unsigned int startIndex, uint32_t value, unsigned int* endIndex);
            static unsigned char* serializeInt64(unsigned char* dest, unsigned int startIndex, uint64_t value, unsigned int* endIndex);
            static unsigned char* serializeString(unsigned char* dest, unsigned int startIndex, std::string str, unsigned int* endIndex);

            static uint32_t deserializeInt32(unsigned char* bytes, unsigned int* startIndex);
            static uint64_t deserializeInt64(unsigned char* bytes, unsigned int* startIndex);
            static std::string deserializeString(unsigned char* bytes, unsigned int* startIndex, uint32_t length);
        };
    }
}

#endif	// SERIALIZER_H
