#include <shadapp/Core.h>
#include <shadapp/data/Compression.h>
#include <shadapp/data/Serializer.h>
#include <shadapp/protocol/RequestMessage.h>
#include <shadapp/protocol/ResponseMessage.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/fs/FileSplitter.h>

#include <cstring>
#include <iostream>
#include <vector>

#include <shadapp/Network.h>
#include <shadapp/data/Hash.h>

namespace shadapp {

    namespace protocol {

        RequestMessage::RequestMessage(
                std::bitset<4> version,
                std::bitset<12> id,
                std::string folder,
                std::string name,
                uint64_t offset,
                uint32_t size)
        : AbstractMessage(id, version, Type::REQUEST, true),
        folder(folder.substr(0, MAX_FOLDERNAME_SIZE)),
        name(name.substr(0, MAX_FILENAME_SIZE)),
        offset(offset),
        size(size) {
        }

        RequestMessage::RequestMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            if (isCompressed()) {
                shadapp::data::MsgCompresser::decompress(bytes);
            }
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
            if (isCompressed()) {
                shadapp::data::MsgCompresser::compress(bytes);
            }
            return bytes;
        }

        void RequestMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {            
            //search folder
            for(auto &forFolder : lp.getConfig()->getFolders()){
                if(forFolder->getId().compare(folder) == 0){
                    for(auto &fileInf : forFolder->getFileInfos()){
                        if(getName().compare(fileInf.getName()) == 0){
                            shadapp::fs::FileSplitter splitter(lp.getConfig()->getFoldersPath() + forFolder->getPath() + fileInf.getName());
                            std::string strData(splitter.getBlock(offset,size).data());
                            strData = strData.substr(0, size);
                            shadapp::protocol::ResponseMessage responseMessage(*(lp.getConfig()->getVersion()), getId(), strData);
                            std::string hash;
                            std::vector<char> block = splitter.getBlock(offset, MAX_BLOCK_SIZE);
                            shadapp::data::Hash256::hash(reinterpret_cast<uint8_t*> (&block[0]), block.size(), hash);
                            //Logger::debug("HASH send : %s ", hash.c_str());
                            lp.getNetwork()->send(device.getSocket(), responseMessage);
                        }
                    }
                }
            }
        }
        
    }
}
