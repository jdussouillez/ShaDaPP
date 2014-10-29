#include <shadapp/protocol/PongMessage.h>

namespace shadapp {

    namespace protocol {

        PongMessage::PongMessage(std::bitset<4> version, const PingMessage& ping)
        : Message(ping.getId(), version, Type::PONG, false) {
        }

        PongMessage::PongMessage(std::vector<uint8_t>* bytes)
        : Message(bytes) {
        }
    }
}
