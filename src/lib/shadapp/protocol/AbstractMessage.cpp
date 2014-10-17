#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        AbstractMessage::AbstractMessage(std::bitset<4> version, Type type, bool compressed)
        : version(version), type(type), compressed(compressed) {
        }

        AbstractMessage::AbstractMessage(std::bitset<12> id, std::bitset<4> version, Type type, bool compressed)
        : AbstractMessage(version, type, compressed) {
            this->id = id;
        }

        AbstractMessage::AbstractMessage(unsigned char* bytes)
        : version(0 | (bytes[0] >> 4)),
        id(0 | ((bytes[0] & 0xF) << 8) | bytes[1]),
        type((Type) bytes[2]),
        compressed(bytes[3] & 0x1) {
        }

        std::bitset<4> AbstractMessage::getVersion() const {
            return version;
        }

        std::bitset<12> AbstractMessage::getId() const {
            return id;
        }

        Type AbstractMessage::getType() const {
            return type;
        }

        bool AbstractMessage::isCompressed() const {
            return compressed;
        }

        unsigned char* AbstractMessage::serialize(unsigned char* dest, unsigned int* size) const {
            if (dest == nullptr || size == nullptr) {
                return nullptr;
            }
            unsigned long longId = id.to_ulong();
            dest[0] = 0 | (version.to_ulong() << 4) | (longId >> 8);
            dest[1] = 0 | (longId & 0xFF);
            dest[2] = 0 | type;
            dest[3] = compressed; // 7 bits reserved and the last bit for the compression
            *size = 4;
            return dest;
        }
    }
}
