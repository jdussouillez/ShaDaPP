#include <shadapp/protocol/CloseMessage.h>

namespace shadapp {

    namespace protocol {

        CloseMessage::CloseMessage(std::bitset<4> version, std::string reason)
        : AbstractMessage(version, Type::CLOSE, false),
        reason(reason.substr(0, MAX_STR_LENGTH)) {
        }

        CloseMessage::CloseMessage(unsigned char* bytes)
        : AbstractMessage(bytes) {
            unsigned int startIndex = 4;
            uint32_t length = Serializer::deserializeInt32(bytes, &startIndex);
            reason = Serializer::deserializeString(bytes, &startIndex, length);
        }

        std::string CloseMessage::getReason() const {
            return reason;
        }

        unsigned char* CloseMessage::serialize(unsigned char* dest, unsigned int* size) const {
            if (AbstractMessage::serialize(dest, size) == nullptr) {
                return nullptr;
            }
            uint32_t length = reason.size();
            Serializer::serializeInt32(dest, *size, length, size);
            Serializer::serializeString(dest, *size, reason, size);
            return dest;
        }
    }
}
