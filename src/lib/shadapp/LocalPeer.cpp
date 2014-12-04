#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <iostream>

#include <shadapp/LocalPeer.h>
#include <shadapp/config/ConfigReader.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/Network.h>
#include <shadapp/protocol/IndexMessage.h>
#include <shadapp/fs/FileInfo.h>


namespace shadapp {

    LocalPeer::LocalPeer(shadapp::fs::Device* device, std::string configFilePath) {
        config = shadapp::config::ConfigReader::parse(std::string(configFilePath),
                "src/resources/shadapp/configSchema.xsd");
        network = new Network(0, this);
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

    void LocalPeer::sendAllIndexMessage(shadapp::fs::Device *device, std::vector<shadapp::fs::Folder> folders) {
        for (auto &folder : folders) {
            sendIndexMessage(device, folder);
        }
    }

    void LocalPeer::sendIndexMessage(shadapp::fs::Device *device, shadapp::fs::Folder folder) {
        std::vector<shadapp::fs::FileInfo> files;
        //create one fileInfo
        uint32_t flag;
        uint64_t modified;
        uint64_t version;
        uint64_t localVersion;
        std::vector<shadapp::fs::BlockInfo> blocks;
        std::string data = "coucou";
        uint32_t size = 6;
        shadapp::fs::BlockInfo bi(data, size);
        blocks.push_back(bi);
        shadapp::fs::FileInfo fileInf(
                "file1",
                flag,
                modified,
                version,
                localVersion,
                blocks);
        //

        files.push_back(fileInf);
        shadapp::protocol::IndexMessage msg(*config->getVersion(), folder.getId(), files);
        std::cout << "send IM" << folder.getId() << " to : " << device->getId() << std::endl;
        std::cout << "send IM" << sizeof (msg) << std::endl;
        network->send(device->getSocket(), msg);
    }


}

