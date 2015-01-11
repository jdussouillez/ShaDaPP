#include <shadapp/Core.h>
#include <shadapp/protocol/IndexMessage.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/protocol/RequestMessage.h>
#include <shadapp/fs/RequestedBlock.h>

#include <fstream>
#include <iostream>
#include <stdio.h>

#include <shadapp/Network.h>

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

        void IndexMessage::createEmptyFile(shadapp::LocalPeer& lp, shadapp::fs::Folder* folder, shadapp::fs::FileInfo& fileInfo) const {
            uint64_t fileSize = 0;
            for (auto &block : fileInfo.getBlocks()) {
                fileSize += block.getSize();
            }
            std::ofstream file(lp.getConfig()->getFoldersPath() + folder->getPath() + fileInfo.getName());
            for (uint64_t i = 0; i < fileSize; i++) {
                file << "0";
            }
        }

        void IndexMessage::downloadFile(shadapp::LocalPeer& lp, shadapp::fs::Device& device, shadapp::fs::RequestedBlock* reqBlock) const {
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

        void IndexMessage::createRequestedBlock(shadapp::LocalPeer& lp, shadapp::fs::Folder& folder, shadapp::fs::FileInfo* fileInfo) const {
            uint64_t offset = 0;
            //TODO: fix memory
            long unsigned int* totalBlocks = new long unsigned int;
            *totalBlocks = fileInfo->getBlocks().size();
            for (auto &block : fileInfo->getBlocks()) {
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

        void IndexMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
            Logger::info("[INDEXMESSAGE] { ");
            Logger::info("                  Folder : %s", getFolder().c_str());
            //identify the folder
            shadapp::fs::Folder* indexFolder;
            for (auto &folder : lp.getConfig()->getFolders()) {
                if (folder->getId().compare(this->getFolder()) == 0) {
                    indexFolder = folder;
                    indexFolder->setPath(folder->getId() + "/");
                }
            }

            QString path((lp.getConfig()->getFoldersPath() + indexFolder->getPath()).c_str());
            QDir dir(path);
            if (dir.mkdir(path)) { //false if dir already exists, return true if dir was created with success                
                //create files & add fileInfos to local folder
                for (auto fileInfo : getFiles()) {
                    Logger::info("                  => %s", fileInfo.getName().c_str());
                    //TODO: Set flag invalid
                    //TODO: fix memory
                    shadapp::fs::FileInfo* fi = new shadapp::fs::FileInfo(
                    fileInfo.getName(),
                    0,
                    0,
                    fileInfo.getVersion(),
                    0,
                    fileInfo.getBlocks());
                    //std::cout << "test :!:" << fi->getName() << std::endl;
                    indexFolder->addFileInfo(*fi);
                    createEmptyFile(lp, indexFolder, *fi);
                    createRequestedBlock(lp, *indexFolder, fi);
                }
                Logger::info("}");
            } else {
                for (auto &messageFileInfo : getFiles()) {
                    QString path(indexFolder->getPath().c_str());
                    std::string pathFile = lp.getConfig()->getFoldersPath();
                    pathFile += indexFolder->getPath();
                    QDir dir(QString(pathFile.c_str()));
                    QFileInfoList filesInfos = dir.entryInfoList();
                    bool exist = false;
                    // parse directory files
                    for (auto &localFileInfo : filesInfos) {
                        // if a file already exists
                        if (localFileInfo.fileName().toStdString().compare(messageFileInfo.getName()) == 0) {
                            exist = true;
                            // parse localFileInfos to find localFileInfo
                            for (auto &localFileInfo : indexFolder->getFileInfos()) {
                                if (localFileInfo.getName().compare(messageFileInfo.getName()) == 0) {
                                    // check if files have the same block size
                                    if (messageFileInfo.getBlocks().size() == localFileInfo.getBlocks().size()) {
                                        //for each block compare the hashes
                                        for (unsigned long i = 0; i < messageFileInfo.getBlocks().size(); i++) {
                                            //if hashes are different
                                            if (messageFileInfo.getBlocks().at(i).getHash().compare(localFileInfo.getBlocks().at(i).getHash()) != 0) {
                                                //DOWNLOAD
                                                //detect which file to save (conflict))
                                                if (messageFileInfo.getVersion() > localFileInfo.getVersion()) {
                                                    localFileInfo.setVersion(messageFileInfo.getVersion());
                                                    remove(pathFile.c_str());
                                                    //                                                    downloadFile(&messageFileInfo, *indexFolder, device, lp);
                                                } else if (messageFileInfo.getModified() > localFileInfo.getModified()) {
                                                    localFileInfo.setVersion(messageFileInfo.getVersion());
                                                    remove(pathFile.c_str());
                                                    //                                                    downloadFile(&messageFileInfo, *indexFolder, device, lp);
                                                }
                                            }
                                        }
                                    } else {
                                        //DOWNLOAD
                                        if (messageFileInfo.getVersion() > localFileInfo.getVersion()) {
                                            localFileInfo.setVersion(messageFileInfo.getVersion());
                                            remove(pathFile.c_str());
                                            //                                            downloadFile(&messageFileInfo, *indexFolder, device, lp);
                                        } else if (messageFileInfo.getModified() > localFileInfo.getModified()) {
                                            localFileInfo.setVersion(messageFileInfo.getVersion());
                                            remove(pathFile.c_str());
                                            //downloadFile(&messageFileInfo, *indexFolder, device, lp);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (!exist) {
                        //DOWNLOAD MISSING FILE
                        indexFolder->addFileInfo(messageFileInfo);
                        //                        downloadFile(&messageFileInfo, *indexFolder, device, lp);
                    }
                }
            }
            for (auto &reqBlock : lp.getRequestedBlocks()) {
                shadapp::fs::RequestedBlock* rb = reqBlock.second;
                downloadFile(lp, device, reqBlock.second);
            }            
        }
    }
}
