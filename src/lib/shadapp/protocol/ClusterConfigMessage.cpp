#include <shadapp/protocol/ClusterConfigMessage.h>

namespace shadapp {

    namespace protocol {

        ClusterConfigMessage::ClusterConfigMessage(
                std::bitset<4> version,
                std::string clientName,
                std::string clientVersion,
                std::vector<shadapp::fs::Folder> folders,
                std::map<std::string,
                std::string> options)
        : AbstractMessage(version, Type::CLUSTER_CONFIG, false),
        clientName(clientName),
        clientVersion(clientVersion),
        folders(folders),
        options(options) {
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
    }
}
