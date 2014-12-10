#include <shadapp/protocol/AbstractMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        AbstractMessage::AbstractMessage(std::bitset<4> version, Type type, bool compressed)
        : version(version), type(type), compressed(compressed) {
        }

        AbstractMessage::AbstractMessage(std::bitset<12> id, std::bitset<4> version, Type type, bool compressed)
        : AbstractMessage(version, type, compressed) {
            this->id = id;
        }

        AbstractMessage::AbstractMessage(std::vector<uint8_t>& bytes) {
            uint8_t firstByte = shadapp::data::Serializer::deserializeInt8(bytes);
            version = 0 | (firstByte >> 4);
            id = 0 | ((firstByte & 0xF) << 8) | shadapp::data::Serializer::deserializeInt8(bytes);
            type = static_cast<Type> (shadapp::data::Serializer::deserializeInt8(bytes));
            compressed = static_cast<bool> (shadapp::data::Serializer::deserializeInt8(bytes) & 0x1);
            // Message's length. Used to read the socket's content. Not used here
            shadapp::data::Serializer::deserializeInt32(bytes);
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

        std::vector<uint8_t> AbstractMessage::serialize() const {
            std::vector<uint8_t> bytes;
            unsigned long longId = id.to_ulong();
            shadapp::data::Serializer::serializeInt8(bytes, static_cast<uint8_t> (version.to_ulong() << 4 | longId >> 8));
            shadapp::data::Serializer::serializeInt8(bytes, static_cast<uint8_t> (longId & 0xFF));
            shadapp::data::Serializer::serializeInt8(bytes, static_cast<uint8_t> (type));
            shadapp::data::Serializer::serializeInt8(bytes, static_cast<uint8_t> (compressed ? 0x1 : 0x0F));
            shadapp::data::Serializer::serializeInt32(bytes, 8); // Message's length
            return bytes;
        }

        Type AbstractMessage::getType(const std::vector<uint8_t>& bytes) {
            return static_cast<Type> (bytes[2]);
        }
    }
}
