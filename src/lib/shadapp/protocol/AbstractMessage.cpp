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

        AbstractMessage::AbstractMessage(std::vector<uint8_t>* bytes)
        : version(0 | (bytes->at(0) >> 4)),
        id(0 | ((bytes->at(0) & 0xF) << 8) | bytes->at(1)),
        type((Type) bytes->at(2)),
        compressed(bytes->at(3) & 0x1) {
            bytes->erase(bytes->begin(), bytes->begin() + 4); // Remove bytes 0 to 3
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

        std::vector<uint8_t>* AbstractMessage::serialize(std::vector<uint8_t>* bytes) const {
            if (bytes == nullptr) {
                return nullptr;
            }
            unsigned long longId = id.to_ulong();
            bytes->clear();
            bytes->push_back(version.to_ulong() << 4 | longId >> 8);
            bytes->push_back(longId & 0xFF);
            bytes->push_back(type);
            bytes->push_back(compressed ? 0x1 : 0x0);
            return bytes;
        }
    }
}
