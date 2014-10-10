#include <shadapp/ClientConfig.h>

namespace shadapp {

    void ClientConfig::setVersion(std::string version) {
        this->version = version;
    }

    void ClientConfig::setPort(unsigned short port) {
        this->port = port;
    }

    void ClientConfig::setName(std::string name) {
        this->name = name;
    }

    void ClientConfig::addFolder(Folder folder) {
        folders.push_back(folder);
    }

    void ClientConfig::addOption(std::string name, std::string value) {
        options[name] = value;
    }

    std::string ClientConfig::getVersion() const {
        return version;
    }

    unsigned short ClientConfig::getPort() const {
        return port;
    }

    std::string ClientConfig::getName() const {
        return name;
    }

    std::vector<Folder> ClientConfig::getFolders() const {
        return folders;
    }

    std::map<std::string, std::string> ClientConfig::getOptions() const {
        return options;
    }
}
