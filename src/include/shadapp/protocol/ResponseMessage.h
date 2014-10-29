#ifndef RESPONSEMESSAGE_H
#define	RESPONSEMESSAGE_H

#include <shadapp/Core.h>
#include <shadapp/protocol/Message.h>

namespace shadapp {

    namespace protocol {

        class ResponseMessage : public Message {
        private:
            std::string data;

        public:
            explicit ResponseMessage(std::bitset<4> version, std::string data);
            explicit ResponseMessage(std::vector<uint8_t>* bytes);

            std::string getData() const;

            std::vector<uint8_t>* serialize(std::vector<uint8_t>* bytes) const override;
        };
    }
}

#endif	// RESPONSEMESSAGE_H 
