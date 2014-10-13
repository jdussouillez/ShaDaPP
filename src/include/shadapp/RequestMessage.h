#ifndef REQUESTMESSAGE_H
#define	REQUESTMESSAGE_H

#include <bitset>
#include <stdint.h>
#include <string>

#include <shadapp/AbstractMessage.h>

namespace shadapp {

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
    };
}

#endif	// REQUESTMESSAGE_H 
