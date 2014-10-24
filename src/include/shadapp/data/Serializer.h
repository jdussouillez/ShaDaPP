#ifndef SERIALIZER_H
#define	SERIALIZER_H

#include <cstdint>
#include <string>
#include <vector>

namespace shadapp {

    namespace data {

        class Serializer {
        public:
            static std::vector<uint8_t>* serializeInt32(std::vector<uint8_t>* bytes, uint32_t value);
            static std::vector<uint8_t>* serializeInt64(std::vector<uint8_t>* bytes, uint64_t value);
            static std::vector<uint8_t>* serializeString(std::vector<uint8_t>* bytes, std::string str);

            static uint32_t deserializeInt32(std::vector<uint8_t>* bytes);
            static uint64_t deserializeInt64(std::vector<uint8_t>* bytes);
            static std::string deserializeString(std::vector<uint8_t>* bytes, uint32_t length);
        };
    }
}

#endif	// SERIALIZER_H
