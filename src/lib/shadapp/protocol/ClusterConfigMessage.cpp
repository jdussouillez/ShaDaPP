#include <shadapp/Core.h>
#include <shadapp/data/Serializer.h>
#include <shadapp/protocol/ClusterConfigMessage.h>

namespace shadapp {

    namespace protocol {

        ClusterConfigMessage::ClusterConfigMessage(
                std::bitset<4> version,
                std::string clientName,
                std::string clientVersion,
                std::vector<shadapp::fs::Folder> folders,
                std::map<std::string, std::string> options)
        : AbstractMessage(version, Type::CLUSTER_CONFIG, false),
        clientName(clientName.substr(0, MAX_STR_LENGTH)),
        clientVersion(clientVersion.substr(0, MAX_STR_LENGTH)),
        folders(folders),
        options(options) {
        }

        ClusterConfigMessage::ClusterConfigMessage(unsigned char* bytes)
        : AbstractMessage(bytes) {
            unsigned int startIndex = 4;
            uint32_t size;
            size = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            clientName = shadapp::data::Serializer::deserializeString(bytes, &startIndex, size);
            size = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            clientVersion = shadapp::data::Serializer::deserializeString(bytes, &startIndex, size);
            uint32_t nbFolders = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            for (uint32_t i = 0; i < nbFolders; i++) {
                folders.push_back(shadapp::fs::Folder::getFromBytes(bytes, &startIndex));
            }
            uint32_t nbOptions = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            for (uint32_t i = 0; i < nbOptions; i++) {
                size = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
                std::string key = shadapp::data::Serializer::deserializeString(bytes, &startIndex, size);
                size = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
                std::string value = shadapp::data::Serializer::deserializeString(bytes, &startIndex, size);
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

        unsigned char* ClusterConfigMessage::serialize(unsigned char* dest, unsigned int* size) const {
            if (AbstractMessage::serialize(dest, size) == nullptr) {
                return nullptr;
            }
            uint32_t clientNameLength = clientName.size();
            shadapp::data::Serializer::serializeInt32(dest, *size, clientNameLength, size);
            shadapp::data::Serializer::serializeString(dest, *size, clientName, size);
            uint32_t clientVersionLength = clientVersion.size();
            shadapp::data::Serializer::serializeInt32(dest, *size, clientVersionLength, size);
            shadapp::data::Serializer::serializeString(dest, *size, clientVersion, size);
            uint32_t nbFolders = folders.size();
            shadapp::data::Serializer::serializeInt32(dest, *size, nbFolders, size);
            for (auto folder : folders) {
                folder.serialize(dest, size);
            }
            uint32_t nbOptions = options.size();
            shadapp::data::Serializer::serializeInt32(dest, *size, nbOptions, size);
            for (auto option : options) {
                uint32_t keyLength = option.first.length();
                shadapp::data::Serializer::serializeInt32(dest, *size, keyLength, size);
                shadapp::data::Serializer::serializeString(dest, *size, option.first, size);
                uint32_t valueLength = option.second.size();
                shadapp::data::Serializer::serializeInt32(dest, *size, valueLength, size);
                shadapp::data::Serializer::serializeString(dest, *size, option.second, size);
            }
            return dest;
        }
    }
}
