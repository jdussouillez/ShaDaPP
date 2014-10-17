#ifndef CLOSEMESSAGE_H
#define	CLOSEMESSAGE_H

#include <bitset>
#include <string>

#include <shadapp/Core.h>
#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        class CloseMessage : public AbstractMessage {
        private:
            std::string reason;

        public:
            explicit CloseMessage(std::bitset<4> version, std::string reason);
            explicit CloseMessage(unsigned char* bytes);

            std::string getReason() const;
            
            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;
        };
    }
}

#endif	// CLOSEMESSAGE_H
