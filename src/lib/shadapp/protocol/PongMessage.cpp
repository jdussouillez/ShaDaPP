#include <shadapp/protocol/PongMessage.h>

namespace shadapp {

    namespace protocol {

        PongMessage::PongMessage(std::bitset<4> version, const PingMessage& ping)
        : AbstractMessage(ping.getId(), version, Type::PONG, false) {
        }

        PongMessage::PongMessage(unsigned char* bytes)
        : AbstractMessage(bytes) {
        }
    }
}
