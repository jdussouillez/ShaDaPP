#include <shadapp/protocol/PongMessage.h>
#include <shadapp/LocalPeer.h>
#include <iostream>

namespace shadapp {

    namespace protocol {

        PongMessage::PongMessage(std::bitset<4> version, const PingMessage& ping)
        : AbstractMessage(ping.getId(), version, Type::PONG, false) {
        }

        PongMessage::PongMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
        }

        void PongMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
            std::cout << "PONG" << std::endl;
        }
    }
}
