#include <shadapp/protocol/CloseMessage.h>
#include <shadapp/data/Serializer.h>

namespace shadapp {

    namespace protocol {

        CloseMessage::CloseMessage(std::bitset<4> version, std::string reason)
        : AbstractMessage(version, Type::CLOSE, false),
        reason(reason.substr(0, MAX_STR_LENGTH)) {
        }

        CloseMessage::CloseMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            uint32_t length = shadapp::data::Serializer::deserializeInt32(bytes);
            reason = shadapp::data::Serializer::deserializeString(bytes, length);
        }

        std::string CloseMessage::getReason() const {
            return reason;
        }

        std::vector<uint8_t> CloseMessage::serialize() const {
            std::vector<uint8_t> bytes = AbstractMessage::serialize();
            shadapp::data::Serializer::serializeInt32(bytes, reason.length());
            shadapp::data::Serializer::serializeString(bytes, reason);
            return bytes;
        }
    }
}
