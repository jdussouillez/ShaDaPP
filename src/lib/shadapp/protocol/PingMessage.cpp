#include <shadapp/protocol/PingMessage.h>

namespace shadapp {

    namespace protocol {

        PingMessage::PingMessage(std::bitset<4> version)
        : AbstractMessage(version, Type::PING, false) {
        }

        PingMessage::PingMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
        }
    }
}
