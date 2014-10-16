#ifndef RESPONSEMESSAGE_H
#define	RESPONSEMESSAGE_H

#include <bitset>

#include <shadapp/Core.h>
#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        class ResponseMessage : public AbstractMessage {
        private:
            char data[BLOCK_SIZE];

        public:
            ResponseMessage(std::bitset<4> version);

            const char* getData() const;
        };
    }
}

#endif	// RESPONSEMESSAGE_H 
