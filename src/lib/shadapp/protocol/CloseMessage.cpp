#include <cstring>

#include <shadapp/protocol/CloseMessage.h>

namespace shadapp {

    namespace protocol {

        CloseMessage::CloseMessage(std::bitset<4> version, const char* reason)
        : AbstractMessage(version, Type::CLOSE, false) {
            std::strncpy(this->reason, reason, MAX_STR_LENGTH);
        }

        const char* CloseMessage::getReason() const {
            return reason;
        }
    }
}
