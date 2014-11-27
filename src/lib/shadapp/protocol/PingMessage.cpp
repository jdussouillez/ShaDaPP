#include <shadapp/protocol/PingMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        PingMessage::PingMessage(std::bitset<4> version)
        : Message(version, Type::PING, false) {
        }

        PingMessage::PingMessage(std::vector<uint8_t>* bytes)
        : Message(bytes) {
        }

        void PingMessage::executeAction(shadapp::fs::Device &device, shadapp::LocalPeer &lp) const {
//        std::cout << "receive ping" << std::endl;
//        shadapp::protocol::PongMessage pong(ping.getVersion(), ping);
//        this->send(sock, pong);
        }
    }
}
