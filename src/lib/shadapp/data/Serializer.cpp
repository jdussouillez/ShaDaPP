#include <shadapp/data/Serializer.h>

namespace shadapp {

    namespace data {

        void Serializer::serializeInt8(std::vector<uint8_t>& bytes, uint8_t value) {
            bytes.push_back(value);
        }

        void Serializer::serializeInt32(std::vector<uint8_t>& bytes, uint32_t value) {
            int shift = 24;
            uint32_t mask = 0xFF000000;
            for (std::size_t i = 0; i < sizeof(uint32_t); i++) {
                bytes.push_back(static_cast<uint8_t>((value & mask) >> shift));
                shift -= 8;
                mask >>= 8;
            }
        }

        void Serializer::serializeInt64(std::vector<uint8_t>& bytes, uint64_t value) {
            int shift = 56;
            uint64_t mask = 0xFF00000000000000;
            for (std::size_t i = 0; i < sizeof(uint64_t); i++) {
                bytes.push_back(static_cast<uint8_t>((value & mask) >> shift));
                shift -= 8;
                mask >>= 8;
            }
        }

        void Serializer::serializeString(std::vector<uint8_t>& bytes, std::string str) {
            for (std::size_t i = 0; i < str.length(); i++) {
                bytes.push_back(static_cast<uint8_t>(str.at(i)));
            }
        }

        uint8_t Serializer::deserializeInt8(std::vector<uint8_t>& bytes) {
            uint8_t value = bytes[0];
            bytes.erase(bytes.begin(), bytes.begin() + 1);
            return value;
        }

        uint32_t Serializer::deserializeInt32(std::vector<uint8_t>& bytes) {
            int shift = 24;
            uint32_t integer = 0;
            for (std::size_t i = 0; i < sizeof(uint32_t); i++) {
                integer |= bytes[0] << shift;
                bytes.erase(bytes.begin());
                shift -= 8;
            }
            return integer;
        }

        uint64_t Serializer::deserializeInt64(std::vector<uint8_t>& bytes) {
            int shift = 56;
            uint32_t integer = 0;
            for (std::size_t i = 0; i < sizeof(uint64_t); i++) {
                integer |= bytes[0] << shift;
                bytes.erase(bytes.begin());
                shift -= 8;
            }
            return integer;
        }

        std::string Serializer::deserializeString(std::vector<uint8_t>& bytes, uint32_t length) {
            std::string str(bytes.begin(), bytes.begin() + length);
            bytes.erase(bytes.begin(), bytes.begin() + length);
            return str;
        }
    }
}
