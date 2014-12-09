#ifndef BLOCKINFO_H
#define	BLOCKINFO_H

#include <string>

#include <shadapp/Core.h>
#include <shadapp/data/Serializable.h>
#include <shadapp/fs/Device.h>

namespace shadapp {

    namespace fs {

        class BlockInfo : public shadapp::data::Serializable {
        private:
            uint32_t size;
            std::string hash;
            shadapp::fs::Device* downloadPeer;

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
