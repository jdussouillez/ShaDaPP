#include <algorithm>
#include <iostream>

#include <QObject>

#include <shadapp/Logger.h>
#include <shadapp/Core.h>
#include <shadapp/data/Compression.h>
#include <shadapp/data/Serializer.h>
#include <shadapp/protocol/ClusterConfigMessage.h>
#include <shadapp/LocalPeer.h>

#include "shadapp/Network.h"

namespace shadapp {

    namespace protocol {

        ClusterConfigMessage::ClusterConfigMessage(
                std::bitset<4> version,
                std::string clientName,
                std::string clientVersion,
                std::vector<shadapp::fs::Folder*> folders,
                std::map<std::string, std::string> options)
        : AbstractMessage(version, Type::CLUSTER_CONFIG, true),
        clientName(clientName.substr(0, MAX_STR_LENGTH)),
        clientVersion(clientVersion.substr(0, MAX_STR_LENGTH)),
        folders(folders),
        options(options) {
            for (auto folder : folders) {
                for (auto device : folder->getDevices()) {
                    Logger::debug("device : %s", device->getName().c_str());
                }
            }

        }

        ClusterConfigMessage::ClusterConfigMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            if (isCompressed()) {
                shadapp::data::MsgCompresser::decompress(bytes);
            }
            uint32_t size;
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            clientName = shadapp::data::Serializer::deserializeString(bytes, size);
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            clientVersion = shadapp::data::Serializer::deserializeString(bytes, size);
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            for (uint32_t i = 0; i < size; i++) {
                folders.push_back(new shadapp::fs::Folder(bytes));
            }
            uint32_t nbOptions = shadapp::data::Serializer::deserializeInt32(bytes);
            for (uint32_t i = 0; i < nbOptions; i++) {
                size = shadapp::data::Serializer::deserializeInt32(bytes);
                std::string key = shadapp::data::Serializer::deserializeString(bytes, size);
                size = shadapp::data::Serializer::deserializeInt32(bytes);
                std::string value = shadapp::data::Serializer::deserializeString(bytes, size);
                options[key] = value;
            }
            
        }

        std::string ClusterConfigMessage::getClientName() const {
            return clientName;
        }

        std::string ClusterConfigMessage::getClientVersion() const {
            return clientVersion;
        }

        std::vector<shadapp::fs::Folder*> ClusterConfigMessage::getFolders() const {
            return folders;
        }

        std::map<std::string, std::string> ClusterConfigMessage::getOptions() const {
            return options;
        }

        std::vector<uint8_t> ClusterConfigMessage::serialize() const {
            std::vector<uint8_t> bytes = AbstractMessage::serialize();
            shadapp::data::Serializer::serializeInt32(bytes, clientName.length());
            shadapp::data::Serializer::serializeString(bytes, clientName);
            shadapp::data::Serializer::serializeInt32(bytes, clientVersion.length());
            shadapp::data::Serializer::serializeString(bytes, clientVersion);
            shadapp::data::Serializer::serializeInt32(bytes, folders.size());
            for (auto f : folders) {
                std::vector<uint8_t> folderBytes = f->serialize();
                bytes.insert(bytes.end(), folderBytes.begin(), folderBytes.end());
            }
            shadapp::data::Serializer::serializeInt32(bytes, options.size());
            for (auto o : options) {
                shadapp::data::Serializer::serializeInt32(bytes, o.first.length());
                shadapp::data::Serializer::serializeString(bytes, o.first);
                shadapp::data::Serializer::serializeInt32(bytes, o.second.length());
                shadapp::data::Serializer::serializeString(bytes, o.second);
            }
            // Set the message's length
            shadapp::data::Serializer::serializeInt32(bytes, bytes.size(), 4);
            if (isCompressed()) {
                shadapp::data::MsgCompresser::compress(bytes);
            }
            return bytes;
        }

        void ClusterConfigMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
            //update de config
            Logger::info("[CLUSTERCONFIGMESSAGE] {");
            for (auto folder : this->folders) {
                bool exist = false;
                for (auto &configFolder : lp.getConfig()->getFolders()) {
                    if (folder->getId().compare(configFolder->getId()) == 0) {
                        exist = true;
                        shadapp::Logger::info("                         => %s : Already Exist", folder->getId().c_str());
                    }
                }
                if (!exist) { // if the folder don't exist in the config, add it
                    shadapp::Logger::info("                         => %s : Added", folder->getId().c_str());
                    shadapp::fs::Folder* newFolder = new shadapp::fs::Folder(folder->getId(), folder->getId());
                    for (auto folderDevice : folder->getDevices()) {
                        if (folderDevice->getId().compare(lp.getConfig()->getID()) != 0) {
                            shadapp::fs::Device* newDevice = new shadapp::fs::Device(folderDevice->getId(), "",
                                    folderDevice->getAddress(), folderDevice->getPort(),
                                    0, folderDevice->getMaxLocalVersion());
                            newFolder->addDevice(newDevice);
                        }

                    }
                    lp.getConfig()->addFolder(newFolder);
                    QObject::connect(newFolder, SIGNAL(signalFileModify(shadapp::fs::Folder*, shadapp::fs::FileInfo*)), lp.getNetwork(), SLOT(slotSendIndexUpdateMessage(shadapp::fs::Folder*, shadapp::fs::FileInfo*)));
                    QString path((lp.getConfig()->getFoldersPath() + newFolder->getPath()).c_str());
                    QDir dir(path);
                    dir.mkdir(path);
                    newFolder->createFileWatcher(lp.getConfig()->getFoldersPath());
                    newFolder->stopFileWatcher();
                }
            }
            Logger::info("}");
            std::vector<shadapp::fs::Folder*> imFolders;
            for (auto &folder : lp.getConfig()->getFolders()) {
                for (auto &deviceFor : folder->getDevices()) {
                    if (deviceFor->getId().compare(device.getId()) == 0) {
                        imFolders.push_back(folder);
                    }
                }
            }
            lp.sendAllIndexMessage(&device, imFolders);
        }
    }
}
