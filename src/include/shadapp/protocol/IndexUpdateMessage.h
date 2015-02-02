#ifndef INDEXUPDATEMESSAGE_H
#define	INDEXUPDATEMESSAGE_H

#include <shadapp/protocol/AbstractIndexMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class IndexUpdateMessage : public AbstractIndexMessage {
        public:
            explicit IndexUpdateMessage(
                    std::bitset<4> version,
                    std::string folder,
                    std::vector<shadapp::fs::FileInfo> files);
            explicit IndexUpdateMessage(std::vector<uint8_t>& bytes);
            
            virtual void executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const override;
        };
    }
}

#endif	// INDEXUPDATEMESSAGE_H
