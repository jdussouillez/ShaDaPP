#include <shadapp/data/Serializer.h>
#include <shadapp/data/Compression.h>
#include <shadapp/protocol/AbstractIndexMessage.h>

#include <shadapp/Core.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/protocol/RequestMessage.h>
#include <shadapp/fs/RequestedBlock.h>

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <shadapp/data/Compression.h>

#include <shadapp/Network.h>

namespace shadapp {

    namespace protocol {

        AbstractIndexMessage::AbstractIndexMessage(
                Type type,
                std::bitset<4> version,
                std::string folder,
                std::vector<shadapp::fs::FileInfo> files,
                bool compress)
        : AbstractMessage(version, type, compress),
        folder(folder),
        files(files) {
        }

        AbstractIndexMessage::AbstractIndexMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            if(isCompressed()){
                shadapp::data::MsgCompresser::decompress(bytes);                        
            }
            uint32_t size = shadapp::data::Serializer::deserializeInt32(bytes);
            folder = shadapp::data::Serializer::deserializeString(bytes, size);
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            for (uint32_t i = 0; i < size; i++) {
                files.push_back(shadapp::fs::FileInfo(bytes));
            }
        }

        void AbstractIndexMessage::addFile(shadapp::fs::FileInfo file) {
            files.push_back(file);
        }

        std::string AbstractIndexMessage::getFolder() const {
            return folder;
        }

        std::vector<shadapp::fs::FileInfo> AbstractIndexMessage::getFiles() const {
            return files;
        }

        std::vector<uint8_t> AbstractIndexMessage::serialize() const {
            std::vector<uint8_t> bytes = AbstractMessage::serialize();
            shadapp::data::Serializer::serializeInt32(bytes, folder.length());
            shadapp::data::Serializer::serializeString(bytes, folder);
            shadapp::data::Serializer::serializeInt32(bytes, files.size());
            for (auto f : files) {
                std::vector<uint8_t> fileBytes = f.serialize();
                bytes.insert(bytes.end(), fileBytes.begin(), fileBytes.end());
            }
            // Set the message's length
            shadapp::data::Serializer::serializeInt32(bytes, bytes.size(), 4);            
            if (isCompressed()) {
                shadapp::data::MsgCompresser::compress(bytes);
            }            
            return bytes;
        }
        
        void AbstractIndexMessage::createEmptyFile(shadapp::LocalPeer& lp, shadapp::fs::Folder* folder, shadapp::fs::FileInfo& fileInfo) const {
            uint64_t fileSize = 0;
            for (auto &block : fileInfo.getBlocks()) {
                fileSize += block.getSize();
            }
            std::ofstream file(lp.getConfig()->getFoldersPath() + folder->getPath() + fileInfo.getName());
            for (uint64_t i = 0; i < fileSize; i++) {
                file << "0";
            }
        }

        void AbstractIndexMessage::downloadFile(shadapp::LocalPeer& lp, shadapp::fs::Device& device, shadapp::fs::RequestedBlock* reqBlock) const {
            shadapp::protocol::RequestMessage requestMessage(
                    *(lp.getConfig()->getVersion()),
                    reqBlock->getId(),
                    getFolder(),
                    reqBlock->getFileInfo()->getName(),
                    reqBlock->getOffset(),
                    reqBlock->getSize());
            lp.getNetwork()->send(device.getSocket(), requestMessage);
        }


        //        void IndexMessage::downloadFile(shadapp::fs::FileInfo* fileInfo,
        //                shadapp::fs::Folder& folder,
        //                shadapp::fs::Device& device,
        //                shadapp::LocalPeer& lp) const {
        //            //            folder.addFileInfo(fileInfo);
        //            uint64_t offset = 0;
        //            long unsigned int totalBlocks = fileInfo->getBlocks().size();
        //            for (auto &block : fileInfo->getBlocks()) {
        //                std::cout << "taille du block" << block.getSize() << std::endl;
        //                std::bitset<12> id = lp.generateMessageId();
        //
        //                shadapp::fs::RequestedBlock* reqBlock = new shadapp::fs::RequestedBlock(id,
        //                        &folder,
        //                        fileInfo,
        //                        &totalBlocks,
        //                        block.getHash(),
        //                        offset,
        //                        block.getSize());
        //                lp.addRequestedBlock(reqBlock);
        //
        //                shadapp::protocol::RequestMessage requestMessage(
        //                        *(lp.getConfig()->getVersion()),
        //                        id,
        //                        getFolder(),
        //                        fileInfo->getName(),
        //                        offset,
        //                        block.getSize());
        //                lp.getNetwork()->send(device.getSocket(), requestMessage);
        //                offset += block.getSize();
        //            }
        //        }

        void AbstractIndexMessage::createRequestedBlock(shadapp::LocalPeer& lp, shadapp::fs::Folder& folder, shadapp::fs::FileInfo* fileInfo) const {
            uint64_t offset = 0;
            //TODO: fix memory
            long unsigned int* totalBlocks = new long unsigned int;
            *totalBlocks = fileInfo->getBlocks().size();
            for (auto &block : fileInfo->getBlocks()) {
                //Logger::debug("HASH 1 : %s", block.getHash().c_str());
                std::bitset<12> id = lp.generateMessageId();
                shadapp::fs::RequestedBlock* reqBlock = new shadapp::fs::RequestedBlock(id,
                        &folder,
                        fileInfo,
                        totalBlocks,
                        block.getHash(),
                        offset,
                        block.getSize());
                lp.addRequestedBlock(reqBlock);
                offset += block.getSize();
            }
        }

        void AbstractIndexMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
        }
    }
}
