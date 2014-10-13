#include <shadapp/PongMessage.h>

namespace shadapp {

    PongMessage::PongMessage(std::bitset<4> version, const PingMessage& ping)
    : AbstractMessage(ping.getId(), version, Type::PONG, false) {
    }
}
