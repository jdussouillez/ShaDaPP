#ifndef PING_H
#define	PING_H

#include <bitset>

#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        class PingMessage : public AbstractMessage {
        public:
            explicit PingMessage(std::bitset<4> version);
            explicit PingMessage(unsigned char* bytes);
        };
    }
}

#endif	// PING_H
