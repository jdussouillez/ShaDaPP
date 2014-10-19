#ifndef BLOCKINFO_H
#define	BLOCKINFO_H

#include <string>

#include <shadapp/Core.h>
#include <shadapp/data/Serializable.h>

namespace shadapp {

    namespace fs {

        class BlockInfo : public shadapp::data::Serializable {
        private:
            uint32_t size;
            std::string hash;

        public:
            explicit BlockInfo(std::string data, uint32_t size);

            unsigned int getSize() const;
            std::string getHash() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;
            
            static BlockInfo getFromBytes(unsigned char* bytes, unsigned int* size);
        };
    }
}

#endif	// BLOCKINFO_H 
