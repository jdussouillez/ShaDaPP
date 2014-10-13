#include <shadapp/PingMessage.h>

namespace shadapp {

    PingMessage::PingMessage(std::bitset<4> version) : AbstractMessage(version, Type::PING, false) {
    }
}
