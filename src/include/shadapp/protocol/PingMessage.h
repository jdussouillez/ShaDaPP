#ifndef PING_H
#define	PING_H

#include <bitset>

#include <shadapp/protocol/Message.h>

namespace shadapp {

    namespace protocol {

        class PingMessage : public Message {
        public:
            explicit PingMessage(std::bitset<4> version);
            explicit PingMessage(std::vector<uint8_t>* bytes);
        };
    }
}

#endif	// PING_H
