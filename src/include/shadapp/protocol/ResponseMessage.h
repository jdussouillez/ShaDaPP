#ifndef RESPONSEMESSAGE_H
#define	RESPONSEMESSAGE_H

#include <shadapp/Core.h>
#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        class ResponseMessage : public AbstractMessage {
        private:
            std::string data;

        public:
            explicit ResponseMessage(std::bitset<4> version, std::string data);
            explicit ResponseMessage(std::vector<uint8_t>& bytes);

            std::string getData() const;

            std::vector<uint8_t> serialize() const override;
        };
    }
}

#endif	// RESPONSEMESSAGE_H 
