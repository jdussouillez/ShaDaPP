#ifndef CLOSEMESSAGE_H
#define	CLOSEMESSAGE_H

#include <bitset>
#include <string.h>

#include <shadapp/Core.h>
#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        class CloseMessage : public AbstractMessage {
        private:
            char reason[MAX_STR_LENGTH];

        public:
            explicit CloseMessage(std::bitset<4> version, const char* reason);

            const char* getReason() const;
        };
    }
}

#endif	// CLOSEMESSAGE_H
