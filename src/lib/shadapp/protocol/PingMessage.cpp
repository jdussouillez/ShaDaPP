#include <shadapp/protocol/PingMessage.h>

namespace shadapp {

    namespace protocol {

        PingMessage::PingMessage(std::bitset<4> version)
        : AbstractMessage(version, Type::PING, false) {
        }

        PingMessage::PingMessage(unsigned char* bytes)
        : AbstractMessage(bytes) {
        }
    }
}
