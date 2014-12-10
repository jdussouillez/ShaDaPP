#ifndef CLOSEMESSAGE_H
#define	CLOSEMESSAGE_H

#include <bitset>
#include <string>

#include <shadapp/Core.h>
#include <shadapp/protocol/AbstractMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class CloseMessage : public AbstractMessage {
        private:
            std::string reason;

        public:
            explicit CloseMessage(std::bitset<4> version, std::string reason);
            explicit CloseMessage(std::vector<uint8_t>& bytes);

            std::string getReason() const;

            virtual std::vector<uint8_t> serialize() const override;
            virtual void executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const override;
        };
    }
}

#endif	// CLOSEMESSAGE_H
