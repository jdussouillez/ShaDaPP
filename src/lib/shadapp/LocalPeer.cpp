#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QDir>
#include <QString>
#include <QFileInfoList>

#include <chrono>
#include <iostream>

#include <shadapp/LocalPeer.h>
#include <shadapp/Core.h>
#include <shadapp/config/ConfigReader.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/Network.h>
#include <shadapp/protocol/IndexMessage.h>
#include <shadapp/fs/FileInfo.h>
#include <shadapp/fs/FileSplitter.h>


namespace shadapp {

    LocalPeer::LocalPeer(shadapp::fs::Device* device, std::string configFilePath) {
        config = shadapp::config::ConfigReader::parse(std::string(configFilePath),
                "src/resources/shadapp/configSchema.xsd");
        network = new Network(0, this);
        
        for (auto &folder : config->getFolders()) {
            QString path(folder->getPath().c_str());            
            std::string pathFile = config->getFoldersPath();
            pathFile += folder->getPath();
            QDir dir(QString(pathFile.c_str()));
            QFileInfoList filesInfos = dir.entryInfoList();
            for (auto &fileInfo : filesInfos) {
                if (fileInfo.isFile()) {
                    std::cout << "the file path is : " << fileInfo.fileName().toStdString() << std::endl;
                    shadapp::fs::FileSplitter splitter(fileInfo.absoluteFilePath().toStdString());
                    std::vector<shadapp::fs::BlockInfo> blocks;
                    uint64_t offset = 0;
                    for (unsigned int i = 0; i < splitter.getNbBlocks(); i++) {
                        //TODO: CHANGE HASH                        
                        shadapp::fs::BlockInfo blockInfo("HASH", splitter.getBlock(offset, MAX_BLOCK_SIZE).size());
                        blocks.push_back(blockInfo);
                        offset += MAX_BLOCK_SIZE;
                    }
                    std::chrono::milliseconds ms =
                            std::chrono::duration_cast< std::chrono::milliseconds >
                            (std::chrono::high_resolution_clock::now().time_since_epoch());
                    shadapp::fs::FileInfo fInfo(fileInfo.fileName().toStdString(),
                            0, (uint64_t) ms.count(), 0, 0, blocks);
                    std::cout<< "file : " << fInfo.getName() << std::endl;
                    folder->addFileInfo(fInfo);
                }
            }
        }

    }

    shadapp::config::PeerConfig * LocalPeer::getConfig() {
        return config;
    }

    void LocalPeer::start() {
        network->start();
    }

    shadapp::Network* LocalPeer::getNetwork() {
        return network;
    }

    void LocalPeer::sendAllIndexMessage(shadapp::fs::Device *device, std::vector<shadapp::fs::Folder*> folders) {
        for (auto &folder : folders) {
            sendIndexMessage(device, folder);
        }
    }

    void LocalPeer::sendIndexMessage(shadapp::fs::Device *device, shadapp::fs::Folder* folder) {
//        uint32_t flag; // ???
//        uint64_t modified; //
//        uint64_t version; // 0 ?
//        uint64_t localVersion; // 0 ?

        shadapp::protocol::IndexMessage msg(*config->getVersion(), folder->getId(), folder->getFileInfos());
        //std::cout << "send IM" << folder->getId() << " path : " << folder->getPath() << " to : " << device->getId() << std::endl;
        //std::cout << " TEST IM DEBUG : " << device->getSocket()->error() << std::endl;
        network->send(device->getSocket(), msg);
    }

    void LocalPeer::sendPingMessage(shadapp::fs::Device* device) {
        shadapp::protocol::PingMessage ping(*config->getVersion());
        network->send(device->getSocket(), ping);
    }



}

