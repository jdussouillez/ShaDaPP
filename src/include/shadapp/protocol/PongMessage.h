#ifndef PONG_H
#define	PONG_H

#include <bitset>

#include <shadapp/protocol/PingMessage.h>

namespace shadapp {

    namespace protocol {

        class PongMessage : public AbstractMessage {
        public:
            explicit PongMessage(std::bitset<4> version, const PingMessage& ping);
            explicit PongMessage(std::vector<uint8_t>& bytes);
        };
    }
}

#endif	// PONG_H
