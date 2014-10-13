#include <shadapp/ClusterConfigMessage.h>

namespace shadapp {

    ClusterConfigMessage::ClusterConfigMessage(std::string clientName,
            std::string clientVersion,
            std::vector<Folder> folders,
            std::map<std::string,
            std::string> options)
    : clientName(clientName),
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

    std::vector<Folder> ClusterConfigMessage::getFolders() const {
        return folders;
    }

    std::map<std::string, std::string> ClusterConfigMessage::getOptions() const {
        return options;
    }
}
