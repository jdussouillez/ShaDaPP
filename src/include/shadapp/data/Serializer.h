#ifndef SERIALIZER_H
#define	SERIALIZER_H

#include <cstdint>
#include <string>
#include <vector>

namespace shadapp {

    namespace data {

        class Serializer {
        public:
            static void serializeInt8(std::vector<uint8_t>& bytes, uint8_t value);
            static void serializeInt32(std::vector<uint8_t>& bytes, uint32_t value);
            static void serializeInt32(std::vector<uint8_t>& bytes, uint32_t value, int position);
            static void serializeInt64(std::vector<uint8_t>& bytes, uint64_t value);
            static void serializeString(std::vector<uint8_t>& bytes, std::string str);

            static uint8_t deserializeInt8(std::vector<uint8_t>& bytes, bool erase = true);
            static uint32_t deserializeInt32(std::vector<uint8_t>& bytes, bool erase = true);
            static uint64_t deserializeInt64(std::vector<uint8_t>& bytes, bool erase = true);
            static std::string deserializeString(std::vector<uint8_t>& bytes, uint32_t length, bool erase = true);
        };
    }
}

#endif	// SERIALIZER_H
