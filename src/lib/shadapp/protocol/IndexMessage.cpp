#include "shadapp/protocol/IndexMessage.h"
#include <shadapp/LocalPeer.h>

#include <iostream>

namespace shadapp {

    namespace protocol {

        IndexMessage::IndexMessage(
                std::bitset<4> version,
                std::string folder,
                std::vector<shadapp::fs::FileInfo> files)
        : AbstractIndexMessage(Type::INDEX, version, folder, files) {
        }

        IndexMessage::IndexMessage(std::vector<uint8_t>& bytes)
        : AbstractIndexMessage(bytes) {
        }

        void IndexMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
            //AbstractIndexMessage::executeAction()
            for (auto &file : this->getFiles()) {
                std::cout << "file : " << file.getName() << std::endl;
                for (auto &block : file.getBlocks()) {
                    std::cout << "block : " << block.getHash() << std::endl;
                    std::cout << "block size : " << block.getHash().size() << std::endl;
                }
            }
            std::cout << "debug IM" << this->getFolder() << std::endl;
            std::cout << "test" << sizeof (*this) << std::endl;
        }
    }
}
