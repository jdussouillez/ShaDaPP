#ifndef CLOSEMESSAGE_H
#define	CLOSEMESSAGE_H

#include <bitset>
#include <string>

#include <shadapp/Core.h>
#include <shadapp/protocol/Message.h>

namespace shadapp {

    namespace protocol {

        class CloseMessage : public Message {
        private:
            std::string reason;

        public:
            explicit CloseMessage(std::bitset<4> version, std::string reason);
            explicit CloseMessage(std::vector<uint8_t>* bytes);

            std::string getReason() const;

            std::vector<uint8_t>* serialize(std::vector<uint8_t>* bytes) const override;
        };
    }
}

#endif	// CLOSEMESSAGE_H
