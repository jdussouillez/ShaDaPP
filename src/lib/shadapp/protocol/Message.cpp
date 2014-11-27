#include <shadapp/protocol/Message.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        Message::Message(std::bitset<4> version, Type type, bool compressed)
        : version(version), type(type), compressed(compressed) {
        }

        Message::Message(std::bitset<12> id, std::bitset<4> version, Type type, bool compressed)
        : Message(version, type, compressed) {
            this->id = id;
        }
        
        Message::Message(std::vector<uint8_t>* bytes)
        : version(0 | (bytes->at(0) >> 4)),
        id(0 | ((bytes->at(0) & 0xF) << 8) | bytes->at(1)),
        type((Type) bytes->at(2)),
        compressed(bytes->at(3) & 0x1) {
            bytes->erase(bytes->begin(), bytes->begin() + 4); // Remove bytes 0 to 3
        }

        std::bitset<4> Message::getVersion() const {
            return version;
        }

        std::bitset<12> Message::getId() const {
            return id;
        }

        Type Message::getType() const {
            return type;
        }

        bool Message::isCompressed() const {
            return compressed;
        }

        std::vector<uint8_t>* Message::serialize(std::vector<uint8_t>* bytes) const {
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
        
        Type Message::getType(const std::vector<uint8_t>& bytes) {
            return static_cast<Type>(bytes[2]);
        }
    }
}
