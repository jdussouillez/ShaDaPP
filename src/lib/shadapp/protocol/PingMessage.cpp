#include <shadapp/protocol/PingMessage.h>
#include <shadapp/protocol/PongMessage.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/Network.h>

namespace shadapp {

    namespace protocol {

        PingMessage::PingMessage(std::bitset<4> version)
        : Message(version, Type::PING, false) {
        }

        PingMessage::PingMessage(std::vector<uint8_t>* bytes)
        : Message(bytes) {
        }

        void PingMessage::executeAction(shadapp::fs::Device &device, shadapp::LocalPeer &lp) const {
            shadapp::protocol::PongMessage pong(this->getVersion(), *this);
            lp.getNetwork()->send(device.getSocket(), pong);
        }
    }
}
