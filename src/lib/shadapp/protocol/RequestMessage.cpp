#include <shadapp/Core.h>
#include <shadapp/data/Serializer.h>
#include <shadapp/protocol/RequestMessage.h>
#include <shadapp/protocol/ResponseMessage.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/fs/FileSplitter.h>

#include <iostream>

#include "shadapp/Network.h"

namespace shadapp {

    namespace protocol {

        RequestMessage::RequestMessage(
                std::bitset<4> version,
                std::string folder,
                std::string name,
                uint64_t offset,
                uint32_t size)
        : AbstractMessage(version, Type::REQUEST, false),
        folder(folder.substr(0, MAX_FOLDERNAME_SIZE)),
        name(name.substr(0, MAX_FILENAME_SIZE)),
        offset(offset),
        size(size) {
        }

        RequestMessage::RequestMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            uint32_t length;
            length = shadapp::data::Serializer::deserializeInt32(bytes);
            folder = shadapp::data::Serializer::deserializeString(bytes, length);
            length = shadapp::data::Serializer::deserializeInt32(bytes);
            name = shadapp::data::Serializer::deserializeString(bytes, length);
            offset = shadapp::data::Serializer::deserializeInt64(bytes);
            size = shadapp::data::Serializer::deserializeInt32(bytes);
        }

        std::string RequestMessage::getFolder() const {
            return folder;
        }

        std::string RequestMessage::getName() const {
            return name;
        }

        uint64_t RequestMessage::getOffset() const {
            return offset;
        }

        uint32_t RequestMessage::getSize() const {
            return size;
        }

        std::vector<uint8_t> RequestMessage::serialize() const {
            std::vector<uint8_t> bytes = AbstractMessage::serialize();
            shadapp::data::Serializer::serializeInt32(bytes, folder.length());
            shadapp::data::Serializer::serializeString(bytes, folder);
            shadapp::data::Serializer::serializeInt32(bytes, name.length());
            shadapp::data::Serializer::serializeString(bytes, name);
            shadapp::data::Serializer::serializeInt64(bytes, offset);
            shadapp::data::Serializer::serializeInt32(bytes, size);
            // Set the message's length
            shadapp::data::Serializer::serializeInt32(bytes, bytes.size(), 4);
            return bytes;
        }

        void RequestMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {            
            //search folder
            for(auto &forFolder : lp.getConfig()->getFolders()){
                if(forFolder->getId().compare(folder) == 0){
                    for(auto &fileInf : forFolder->getFileInfos()){
                        if(getName().compare(fileInf.getName()) == 0){
                            std::cout<< "debug request message" << std::endl;
                            std::cout<< "get block from file " << lp.getConfig()->getFoldersPath() + forFolder->getPath() + fileInf.getName() << "offset /size " << offset << "/" << size << std::endl;
                            shadapp::fs::FileSplitter splitter(lp.getConfig()->getFoldersPath() + forFolder->getPath() + fileInf.getName());
                            std::string strData(splitter.getBlock(offset,size).data());
                            shadapp::protocol::ResponseMessage responseMessage(*(lp.getConfig()->getVersion()),strData);
                            lp.getNetwork()->send(device.getSocket(), responseMessage);
                        }
                    }
                }
            }
        }
        
    }
}
