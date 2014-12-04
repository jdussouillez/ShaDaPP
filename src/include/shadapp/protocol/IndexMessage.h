#ifndef INDEXMESSAGE_H
#define	INDEXMESSAGE_H

#include <shadapp/protocol/AbstractIndexMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class IndexMessage : public AbstractIndexMessage {
        public:
            explicit IndexMessage(
                    std::bitset<4> version,
                    std::string folder,
                    std::vector<shadapp::fs::FileInfo> files);
            explicit IndexMessage(std::vector<uint8_t>& bytes);

            virtual void executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const override;
        };
    }
}
#endif	// INDEXMESSAGE_H
