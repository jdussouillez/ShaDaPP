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
                Logger::debug("a");
                //create files & add fileInfos to local folder
                for (auto fileInfo : getFiles()) {
                    //check if the peer have the latest version of the file
                    if (fileInfo.getVersion() == fileInfo.getLocalVersion()) {
                        Logger::debug("b");
                        Logger::info("                  => %s : download", fileInfo.getName().c_str());
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
                }

            } else {
                for (auto &messageFileInfo : getFiles()) {
                    if (messageFileInfo.getVersion() == messageFileInfo.getLocalVersion()) {
                        if (messageFileInfo.isDeleted()) {
                        Logger::info("                  => %s : deleted", messageFileInfo.getName().c_str());
                    } else if (messageFileInfo.isInvalid()) {
                        Logger::info("                  => %s : invalid", messageFileInfo.getName().c_str());
                    } else {
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
                                        Logger::debug("1");
                                        // check if files have the same block size
                                        if (messageFileInfo.getBlocks().size() == localFileInfo.getBlocks().size()) {
                                            Logger::debug("2");
                                            //for each block compare the hashes
                                            bool upToDate = true;
                                            for (unsigned long i = 0; i < messageFileInfo.getBlocks().size(); i++) {
                                                //if hashes are different
                                                if (messageFileInfo.getBlocks().at(i).getHash().compare(localFileInfo.getBlocks().at(i).getHash()) != 0) {
                                                    upToDate = false;
                                                    Logger::debug("3 message : %d  local : %d", messageFileInfo.getModified(), localFileInfo.getModified());
                                                    //DOWNLOAD
                                                    //detect which file to save (conflict))
                                                    if (messageFileInfo.getVersion() > localFileInfo.getVersion()) {
                                                        Logger::debug("4");
                                                        Logger::info("                  => %s : download", messageFileInfo.getName().c_str());
                                                        //localFileInfo.setVersion(messageFileInfo.getVersion());
                                                        remove(pathFile.c_str());
                                                        shadapp::fs::FileInfo* newFileInfo = new shadapp::fs::FileInfo(
                                                                messageFileInfo.getName(),
                                                                0,
                                                                localFileInfo.getModified(),
                                                                messageFileInfo.getVersion(),
                                                                localFileInfo.getVersion(),
                                                                messageFileInfo.getBlocks());
                                                        std::remove(indexFolder->getFileInfos().begin(), indexFolder->getFileInfos().end(), localFileInfo);
                                                        indexFolder->addFileInfo(*newFileInfo);
                                                        createEmptyFile(lp, indexFolder, *newFileInfo);
                                                        createRequestedBlock(lp, *indexFolder, newFileInfo);
                                                    } else if (messageFileInfo.getModified() > localFileInfo.getModified()) {
                                                        Logger::info("                  => %s : download", messageFileInfo.getName().c_str());
                                                        Logger::debug("5");
                                                        localFileInfo.setVersion(messageFileInfo.getVersion());
                                                        remove(pathFile.c_str());
                                                        shadapp::fs::FileInfo* newFileInfo = new shadapp::fs::FileInfo(
                                                                messageFileInfo.getName(),
                                                                0,
                                                                localFileInfo.getModified(),
                                                                messageFileInfo.getVersion(),
                                                                localFileInfo.getVersion(),
                                                                messageFileInfo.getBlocks());
                                                        std::remove(indexFolder->getFileInfos().begin(), indexFolder->getFileInfos().end(), localFileInfo);
                                                        indexFolder->addFileInfo(*newFileInfo);
                                                        createEmptyFile(lp, indexFolder, *newFileInfo);
                                                        createRequestedBlock(lp, *indexFolder, newFileInfo);
                                                    }
                                                }
                                            }
                                            if (upToDate) {
                                                Logger::info("                  => %s : up to date", messageFileInfo.getName().c_str());
                                            }
                                        } else {
                                            //DOWNLOAD
                                            if (messageFileInfo.getVersion() > localFileInfo.getVersion()) {
                                                Logger::info("                  => %s : download", messageFileInfo.getName().c_str());
                                                Logger::debug("6");
                                                localFileInfo.setVersion(messageFileInfo.getVersion());
                                                remove(pathFile.c_str());
                                                shadapp::fs::FileInfo* newFileInfo = new shadapp::fs::FileInfo(
                                                        messageFileInfo.getName(),
                                                        0,
                                                        localFileInfo.getModified(),
                                                        messageFileInfo.getVersion(),
                                                        localFileInfo.getVersion(),
                                                        messageFileInfo.getBlocks());
                                                std::remove(indexFolder->getFileInfos().begin(), indexFolder->getFileInfos().end(), localFileInfo);
                                                indexFolder->addFileInfo(*newFileInfo);
                                                createEmptyFile(lp, indexFolder, *newFileInfo);
                                                createRequestedBlock(lp, *indexFolder, newFileInfo);
                                            } else if (messageFileInfo.getModified() > localFileInfo.getModified()) {
                                                Logger::info("                  => %s : download", messageFileInfo.getName().c_str());
                                                Logger::debug("7");
                                                localFileInfo.setVersion(messageFileInfo.getVersion());
                                                remove(pathFile.c_str());
                                                shadapp::fs::FileInfo* newFileInfo = new shadapp::fs::FileInfo(
                                                        messageFileInfo.getName(),
                                                        0,
                                                        localFileInfo.getModified(),
                                                        messageFileInfo.getVersion(),
                                                        localFileInfo.getVersion(),
                                                        messageFileInfo.getBlocks());
                                                //Logger::debug("debug %d", pos);
                                                //indexFolder->getFileInfos().erase();
                                                std::remove(indexFolder->getFileInfos().begin(), indexFolder->getFileInfos().end(), localFileInfo);
                                                //indexFolder->getFileInfos().erase(indexFolder->getFileInfos().begin() + pos);
                                                indexFolder->addFileInfo(*newFileInfo);
                                                createEmptyFile(lp, indexFolder, *newFileInfo);
                                                createRequestedBlock(lp, *indexFolder, newFileInfo);
                                            } else {
                                                Logger::info("                  => %s : up to date", messageFileInfo.getName().c_str());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (!exist) {
                            Logger::info("                  => %s : download", messageFileInfo.getName().c_str());
                            Logger::debug("file %s doesn't exist !", messageFileInfo.getName().c_str());
                            shadapp::fs::FileInfo* newFileInfo = new shadapp::fs::FileInfo(
                                    messageFileInfo.getName(),
                                    0,
                                    0,
                                    messageFileInfo.getVersion(),
                                    0,
                                    messageFileInfo.getBlocks());
                            indexFolder->addFileInfo(*newFileInfo);
                            createEmptyFile(lp, indexFolder, *newFileInfo);
                            createRequestedBlock(lp, *indexFolder, newFileInfo);
                        }
                    }
                    }
                    

                }
            }
            Logger::info("}");
            for (auto &reqBlock : lp.getRequestedBlocks()) {
                downloadFile(lp, device, reqBlock.second);
            }
        }
    }
}
