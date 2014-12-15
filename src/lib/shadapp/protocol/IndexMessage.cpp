#include <shadapp/protocol/IndexMessage.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/protocol/RequestMessage.h>

#include <iostream>

#include "shadapp/Network.h"

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
            
            //TODO: remove test
            for (auto &file : this->getFiles()) {
                uint64_t offset = 0;
                for (auto &block : file.getBlocks()) {
                    shadapp::protocol::RequestMessage request(*(lp.getConfig()->getVersion()),
                    this->getFolder(),
                    file.getName(),
                    offset,
                    block.getSize());
                    offset+=block.getSize();
                    //lp.sendPingMessage(&device);
                    lp.getNetwork()->send(device.getSocket(), request);
                    lp.getNetwork()->send(device.getSocket(), request);
                    lp.getNetwork()->send(device.getSocket(), request);
                    lp.getNetwork()->send(device.getSocket(), request);
                    //lp.sendPingMessage(&device);
                }
            }
            
        }
    }
}
