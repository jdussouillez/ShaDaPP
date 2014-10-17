#ifndef SERIALIZER_H
#define	SERIALIZER_H

#include <cstdint>
#include <string>

namespace shadapp {

    namespace protocol {

        class Serializer {
        public:
            virtual unsigned char* serialize(unsigned char* dest, unsigned int* size) const = 0;

            static unsigned char* serializeInt32(unsigned char* dest, unsigned int offset, uint32_t value, unsigned int* size);
            static unsigned char* serializeInt64(unsigned char* dest, unsigned int offset, uint64_t value, unsigned int* size);
            static unsigned char* serializeString(unsigned char* dest, unsigned int offset, std::string str, unsigned int* size);

            static uint32_t deserializeInt32(unsigned char* bytes, unsigned int* startIndex);
            static uint64_t deserializeInt64(unsigned char* bytes, unsigned int* startIndex);
            static std::string deserializeString(unsigned char* bytes, unsigned int* startIndex, uint32_t length);
        };
    }
}

#endif	// SERIALIZER_H
