#ifndef REQUESTMESSAGE_H
#define	REQUESTMESSAGE_H

#include <stdint.h>
#include <string>


namespace shadapp {

    class RequestMessage {
    private:
        std::string folder;
        std::string name;
        uint64_t offset;
        unsigned int size;

    public:
        explicit RequestMessage(std::string folder,
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
