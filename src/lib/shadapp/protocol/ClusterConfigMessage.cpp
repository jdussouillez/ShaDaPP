#include <iostream>

#include <shadapp/Core.h>
#include <shadapp/data/Serializer.h>
#include <shadapp/protocol/ClusterConfigMessage.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        ClusterConfigMessage::ClusterConfigMessage(
                std::bitset<4> version,
                std::string clientName,
                std::string clientVersion,
                std::vector<shadapp::fs::Folder> folders,
                std::map<std::string, std::string> options)
        : Message(version, Type::CLUSTER_CONFIG, false),
        clientName(clientName.substr(0, MAX_STR_LENGTH)),
        clientVersion(clientVersion.substr(0, MAX_STR_LENGTH)),
        folders(folders),
        options(options) {
        }

        ClusterConfigMessage::ClusterConfigMessage(std::vector<uint8_t>* bytes)
        : Message(bytes) {
            uint32_t size;
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            clientName = shadapp::data::Serializer::deserializeString(bytes, size);
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            clientVersion = shadapp::data::Serializer::deserializeString(bytes, size);
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            for (uint32_t i = 0; i < size; i++) {
                folders.push_back(shadapp::fs::Folder(bytes));
            }
            std::cout << folders.size() << std::endl;
            std::cout << folders[0].getDevices().size() << std::endl;
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

        std::vector<shadapp::fs::Folder> ClusterConfigMessage::getFolders() const {
            return folders;
        }

        std::map<std::string, std::string> ClusterConfigMessage::getOptions() const {
            return options;
        }

        std::vector<uint8_t>* ClusterConfigMessage::serialize(std::vector<uint8_t>* bytes) const {
            if (Message::serialize(bytes) == nullptr) {
                return nullptr;
            }
            shadapp::data::Serializer::serializeInt32(bytes, clientName.length());
            shadapp::data::Serializer::serializeString(bytes, clientName);
            shadapp::data::Serializer::serializeInt32(bytes, clientVersion.length());
            shadapp::data::Serializer::serializeString(bytes, clientVersion);
            shadapp::data::Serializer::serializeInt32(bytes, folders.size());
            for (auto f : folders) {
                f.serialize(bytes);
            }
            shadapp::data::Serializer::serializeInt32(bytes, options.size());
            for (auto o : options) {
                shadapp::data::Serializer::serializeInt32(bytes, o.first.length());
                shadapp::data::Serializer::serializeString(bytes, o.first);
                shadapp::data::Serializer::serializeInt32(bytes, o.second.length());
                shadapp::data::Serializer::serializeString(bytes, o.second);
            }
            return bytes;
        }
        
        void ClusterConfigMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
            std::cout << "DEBUG CCM" << std::endl;
        }
    }
}
