#include <shadapp/protocol/IndexUpdateMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        IndexUpdateMessage::IndexUpdateMessage(
                std::bitset<4> version,
                std::string folder,
                std::vector<shadapp::fs::FileInfo> files)
        : AbstractIndexMessage(Type::INDEX_UPDATE, version, folder, files) {
        }

        IndexUpdateMessage::IndexUpdateMessage(std::vector<uint8_t>* bytes)
        : AbstractIndexMessage(bytes) {
        }
        
        void IndexUpdateMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {

        }
    }
}
