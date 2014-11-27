#ifndef CLOSEMESSAGE_H
#define	CLOSEMESSAGE_H

#include <bitset>
#include <string>

#include <shadapp/Core.h>
#include <shadapp/protocol/Message.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class CloseMessage : public Message {
        private:
            std::string reason;

        public:
            explicit CloseMessage(std::bitset<4> version, std::string reason);
            explicit CloseMessage(std::vector<uint8_t>* bytes);

            std::string getReason() const;

            virtual std::vector<uint8_t>* serialize(std::vector<uint8_t>* bytes) const override;
            virtual void executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const override;
        };
    }
}

#endif	// CLOSEMESSAGE_H
