#include <shadapp/protocol/CloseMessage.h>
#include <shadapp/data/Serializer.h>

namespace shadapp {

    namespace protocol {

        CloseMessage::CloseMessage(std::bitset<4> version, std::string reason)
        : Message(version, Type::CLOSE, false),
        reason(reason.substr(0, MAX_STR_LENGTH)) {
        }

        CloseMessage::CloseMessage(std::vector<uint8_t>* bytes)
        : Message(bytes) {
            uint32_t length = shadapp::data::Serializer::deserializeInt32(bytes);
            reason = shadapp::data::Serializer::deserializeString(bytes, length);
        }

        std::string CloseMessage::getReason() const {
            return reason;
        }

        std::vector<uint8_t>* CloseMessage::serialize(std::vector<uint8_t>* bytes) const {
            if (Message::serialize(bytes) == nullptr) {
                return nullptr;
            }
            shadapp::data::Serializer::serializeInt32(bytes, reason.length());
            shadapp::data::Serializer::serializeString(bytes, reason);
            return bytes;
        }
    }
}
