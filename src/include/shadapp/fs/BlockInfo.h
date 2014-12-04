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
            explicit BlockInfo(std::vector<uint8_t>& bytes);

            uint32_t getSize() const;
            std::string getHash() const;

            std::vector<uint8_t> serialize() const override;
        };
    }
}

#endif	// BLOCKINFO_H 
