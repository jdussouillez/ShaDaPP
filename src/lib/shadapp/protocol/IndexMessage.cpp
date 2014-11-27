#include "shadapp/protocol/IndexMessage.h"
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        IndexMessage::IndexMessage(
                std::bitset<4> version,
                std::string folder,
                std::vector<shadapp::fs::FileInfo> files)
        : AbstractIndexMessage(Type::INDEX, version, folder, files) {
        }

        IndexMessage::IndexMessage(std::vector<uint8_t>* bytes)
        : AbstractIndexMessage(bytes) {
        }
        
        void IndexMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
            //AbstractIndexMessage::executeAction()
        }
    }
}
