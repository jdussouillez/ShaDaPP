#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QDir>
#include <QString>
#include <QFileInfoList>
#include <QObject>

#include <chrono>
#include <iostream>

#include <shadapp/Logger.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/Core.h>
#include <shadapp/config/ConfigReader.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/protocol/RequestMessage.h>
#include <shadapp/Network.h>
#include <shadapp/protocol/IndexMessage.h>
#include <shadapp/fs/FileInfo.h>
#include <shadapp/fs/FileSplitter.h>
#include <shadapp/fs/FileWatcher.h>
#include <shadapp/data/Hash.h>


namespace shadapp {

    LocalPeer::LocalPeer(shadapp::fs::Device* device, std::string configFilePath) {
        config = shadapp::config::ConfigReader::parse(std::string(configFilePath),
                "src/resources/shadapp/configSchema.xsd");        
        //requestedBlocks = new std::map<unsigned long, shadapp::fs::RequestedBlock*>;
        shadapp::Logger::info("Initialize local config");
        for (auto &folder : config->getFolders()) {
            QString path(folder->getPath().c_str());
            std::string pathFile = config->getFoldersPath();
            pathFile += folder->getPath();
            QDir dir(QString(pathFile.c_str()));
            QFileInfoList filesInfos = dir.entryInfoList();
            for (auto &fileInfo : filesInfos) {
                if (fileInfo.isFile()) {
                    shadapp::fs::FileSplitter splitter(fileInfo.absoluteFilePath().toStdString());
                    std::vector<shadapp::fs::BlockInfo> blocks;
                    uint64_t offset = 0;
                    for (unsigned int i = 0; i < splitter.getNbBlocks(); i++) {
                        std::vector<char> block = splitter.getBlock(offset, MAX_BLOCK_SIZE);
                        std::string hash;
                        shadapp::data::Hash256::hash(reinterpret_cast<uint8_t*> (&block[0]), block.size(), hash);
                        shadapp::fs::BlockInfo blockInfo(hash, block.size());
                        blocks.push_back(blockInfo);
                        offset += MAX_BLOCK_SIZE;
                    }
                    qint64 time = fileInfo.lastModified().toMSecsSinceEpoch()/1000;
                    Logger::debug("TIME : %d", time);
//                    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
//                    std::chrono::system_clock::duration dtn = tp.time_since_epoch();
//                    unsigned long timeEpoch = dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
                    shadapp::fs::FileInfo fInfo(fileInfo.fileName().toStdString(),
                            0, (uint64_t) time, 0, 0, blocks);
                    folder->addFileInfo(fInfo);
                }
            }
            
        }
        network = new Network(0, this);
    }

    LocalPeer::~LocalPeer() {
        delete network;
        //delete requestedBlocks;
    }

    shadapp::config::PeerConfig * LocalPeer::getConfig() {
        return config;
    }

    void LocalPeer::start() {        
        network->start();
        for (auto &folder : config->getFolders()) {
            folder->createFileWatcher(config->getFoldersPath());
            QObject::connect(folder, SIGNAL(signalFileModify(shadapp::fs::Folder*, shadapp::fs::FileInfo*)), network, SLOT(slotSendIndexUpdateMessage(shadapp::fs::Folder*, shadapp::fs::FileInfo*)));
        }
    }

    shadapp::Network* LocalPeer::getNetwork() const {
        return network;
    }

    std::map<unsigned long, shadapp::fs::RequestedBlock*> LocalPeer::getRequestedBlocks() {
        return requestedBlocks;
    }

    void LocalPeer::addRequestedBlock(shadapp::fs::RequestedBlock* block) {
        requestedBlocks[block->getId().to_ulong()] = block;
    }
    void LocalPeer::removeRequestedBlock(uint64_t id) {
        requestedBlocks.erase(requestedBlocks.find(id));
    }


    void LocalPeer::sendAllIndexMessage(shadapp::fs::Device *device, std::vector<shadapp::fs::Folder*> folders) {
        for (auto &folder : folders) {
            sendIndexMessage(device, folder);
        }
    }

    void LocalPeer::sendIndexMessage(shadapp::fs::Device *device, shadapp::fs::Folder * folder) {
        //        uint32_t flag; // ???
        //        uint64_t modified; //
        //        uint64_t version; // 0 ?
        //        uint64_t localVersion; // 0 ?

        shadapp::protocol::IndexMessage msg(*config->getVersion(), folder->getId(), folder->getFileInfos());
        //std::cout << "send IM" << folder->getId() << " path : " << folder->getPath() << " to : " << device->getId() << std::endl;
        //std::cout << " TEST IM DEBUG : " << device->getSocket()->error() << std::endl;
        network->send(device->getSocket(), msg);
    }

    void LocalPeer::sendPingMessage(shadapp::fs::Device * device) {
        shadapp::protocol::PingMessage ping(*config->getVersion());
        network->send(device->getSocket(), ping);
    }

    std::bitset<12> LocalPeer::generateMessageId() {
        messageId = std::bitset<12>(messageId.to_ulong() + 1);
        std::bitset<12> id = messageId;
        if (messageId.count() == 12) {
            messageId.reset();
        }
        return id;
    }
    
    

}

