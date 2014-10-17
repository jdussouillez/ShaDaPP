#ifndef REQUESTMESSAGE_H
#define	REQUESTMESSAGE_H

#include <bitset>
#include <stdint.h>
#include <string>

#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        class RequestMessage : public AbstractMessage {
        private:
            std::string folder;
            std::string name;
            uint64_t offset;
            unsigned int size;

        public:
            explicit RequestMessage(
                    std::bitset<4> version,
                    std::string folder,
                    std::string name,
                    uint64_t offset,
                    unsigned int size);

            std::string getFolder() const;
            std::string getName() const;
            uint64_t getOffset() const;
            unsigned int getSize() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;
        };
    }
}

#endif	// REQUESTMESSAGE_H 
