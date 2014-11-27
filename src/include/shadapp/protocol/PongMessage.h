#ifndef PONG_H
#define	PONG_H

#include <bitset>

#include <shadapp/protocol/PingMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class PongMessage : public Message {
        public:
            explicit PongMessage(std::bitset<4> version, const PingMessage& ping);
            explicit PongMessage(std::vector<uint8_t>* bytes);
            
            virtual void executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const override;
        };
    }
}

#endif	// PONG_H
