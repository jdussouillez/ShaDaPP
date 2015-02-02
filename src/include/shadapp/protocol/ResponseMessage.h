#ifndef RESPONSEMESSAGE_H
#define	RESPONSEMESSAGE_H

#include <shadapp/Core.h>
#include <shadapp/protocol/AbstractMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class ResponseMessage : public AbstractMessage {
        private:
            std::string data;

        public:
            explicit ResponseMessage(std::bitset<4> version, std::bitset<12> id, std::string data);
            explicit ResponseMessage(std::vector<uint8_t>& bytes);

            std::string getData() const;

            virtual std::vector<uint8_t> serialize() const override;
            virtual void executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const override;
        };
    }
}

#endif	// RESPONSEMESSAGE_H 
