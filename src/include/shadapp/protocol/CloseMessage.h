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
            explicit CloseMessage(std::vector<uint8_t>& bytes);

            std::string getReason() const;

            std::vector<uint8_t> serialize() const override;
        };
    }
}

#endif	// CLOSEMESSAGE_H
