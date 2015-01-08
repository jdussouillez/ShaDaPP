#ifndef REQUESTMESSAGE_H
#define	REQUESTMESSAGE_H

#include <bitset>
#include <stdint.h>
#include <string>

#include <shadapp/protocol/AbstractMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class RequestMessage : public AbstractMessage {
            
        private:
            std::string folder;
            std::string name;
            uint64_t offset;
            uint32_t size;

        public:
            explicit RequestMessage(
                    std::bitset<4> version,
                    std::bitset<12> id,
                    std::string folder,
                    std::string name,
                    uint64_t offset,
                    uint32_t size);
            explicit RequestMessage(std::vector<uint8_t>& bytes);

            std::string getFolder() const;
            std::string getName() const;
            uint64_t getOffset() const;
            uint32_t getSize() const;

            virtual std::vector<uint8_t> serialize() const override;
            virtual void executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const override;
        };
    }
}

#endif	// REQUESTMESSAGE_H 
