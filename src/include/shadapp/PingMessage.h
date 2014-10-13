#ifndef PING_H
#define	PING_H

#include <bitset>

#include <shadapp/AbstractMessage.h>

namespace shadapp {

    class PingMessage : public AbstractMessage {
    public:
        explicit PingMessage(std::bitset<4> version);
    };
}

#endif	// PING_H
