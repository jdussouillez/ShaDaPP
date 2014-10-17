#ifndef BLOCKINFO_H
#define	BLOCKINFO_H

#include <cstring>

#include <shadapp/Core.h>

namespace shadapp {

    namespace fs {

        class BlockInfo {
        private:
            unsigned int size;
            char hash[HASH_SIZE];

        public:
            explicit BlockInfo(unsigned int size);

            unsigned int getSize() const;
            const char* getHash() const;
        };
    }
}

#endif	// BLOCKINFO_H 
