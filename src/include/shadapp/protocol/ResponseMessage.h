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
            explicit ResponseMessage(unsigned char* bytes);

            std::string getData() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;
        };
    }
}

#endif	// RESPONSEMESSAGE_H 
