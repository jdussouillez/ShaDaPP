#ifndef PONG_H
#define	PONG_H

#include <bitset>

#include <shadapp/AbstractMessage.h>
#include <shadapp/PingMessage.h>

namespace shadapp {

    class PongMessage : public AbstractMessage {
    public:
        explicit PongMessage(std::bitset<4> version, const PingMessage& ping);
    };
}

#endif	// PONG_H
