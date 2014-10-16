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

        void AbstractMessage::serialize(char* dest, int* size) const {
            unsigned long longId = id.to_ulong();
            dest[0] = 0 | (version.to_ulong() << 4) | (longId >> 8);
            dest[1] = 0 | (longId & 0xFF);
            dest[2] = 0 | type;
            dest[3] = compressed;
            *size = 4;
        }
    }
}
