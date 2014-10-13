#ifndef RESPONSEMESSAGE_H
#define	RESPONSEMESSAGE_H

#include <shadapp/Core.h>

namespace shadapp {

    class ResponseMessage {
    private:
        char data[BLOCK_SIZE];
    public:
        const char* getData() const;
    };
}

#endif	// RESPONSEMESSAGE_H 
