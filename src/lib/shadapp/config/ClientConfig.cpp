#include <cstdlib>

#include <shadapp/config/ClientConfig.h>

namespace shadapp {

    namespace config {

        ClientConfig::ClientConfig() : version(NULL) {
        }

        ClientConfig::~ClientConfig() {
            delete version;
        }

        void ClientConfig::setVersion(std::string version) {
            int versionInteger = std::atoi(version.c_str());
            this->version = new std::bitset<4>(versionInteger);
        }

        void ClientConfig::setPort(unsigned short port) {
            this->port = port;
        }

        void ClientConfig::setName(std::string name) {
            this->name = name;
        }

        void ClientConfig::addFolder(shadapp::fs::Folder folder) {
            folders.push_back(folder);
        }

        void ClientConfig::addOption(std::string name, std::string value) {
            options[name] = value;
        }

        std::bitset<4> ClientConfig::getVersion() const {
            return *version;
        }

        unsigned short ClientConfig::getPort() const {
            return port;
        }

        std::string ClientConfig::getName() const {
            return name;
        }

        std::vector<shadapp::fs::Folder> ClientConfig::getFolders() const {
            return folders;
        }

        std::map<std::string, std::string> ClientConfig::getOptions() const {
            return options;
        }
    }
}
