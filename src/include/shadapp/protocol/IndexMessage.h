#ifndef INDEXMESSAGE_H
#define	INDEXMESSAGE_H

#include <shadapp/protocol/AbstractIndexMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        class IndexMessage : public AbstractIndexMessage {
        private:
            void createEmptyFile(shadapp::LocalPeer& lp, shadapp::fs::Folder* folder, shadapp::fs::FileInfo& fileInfo) const;
            void downloadFile(shadapp::LocalPeer& lp, shadapp::fs::Device& device, shadapp::fs::RequestedBlock* reqBlock) const;
            void createRequestedBlock(shadapp::LocalPeer& lp, shadapp::fs::Folder& folder, shadapp::fs::FileInfo* fileInfo)const;
        public:
            explicit IndexMessage(
                    std::bitset<4> version,
                    std::string folder,
                    std::vector<shadapp::fs::FileInfo> files);
            explicit IndexMessage(std::vector<uint8_t>& bytes);
            
            

            virtual void executeAction(shadapp::fs::Device& device,
                shadapp::LocalPeer& lp) const override;
        };
    }
}
#endif	// INDEXMESSAGE_H
