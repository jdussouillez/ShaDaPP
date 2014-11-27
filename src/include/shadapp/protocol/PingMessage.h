#ifndef PING_H
#define	PING_H

#include <bitset>

#include <shadapp/protocol/Message.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class PingMessage : public Message {
        public:
            explicit PingMessage(std::bitset<4> version);
            explicit PingMessage(std::vector<uint8_t>* bytes);
            
            virtual void executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const override;
        };
    }
}

#endif	// PING_H
