#include <cstdlib>

#include <shadapp/config/PeerConfig.h>

namespace shadapp {

    namespace config {

        PeerConfig::PeerConfig() : version(NULL) {
        }

        PeerConfig::~PeerConfig() {
            delete version;
        }

        void PeerConfig::setVersion(std::string version) {
            int versionInteger = std::atoi(version.c_str());
            this->version = new std::bitset<4>(versionInteger);
        }

        void PeerConfig::setPort(unsigned short port) {
            this->port = port;
        }

        void PeerConfig::setName(std::string name) {
            this->name = name;
        }

        void PeerConfig::addFolder(shadapp::fs::Folder folder) {
            folders.push_back(folder);
        }

        void PeerConfig::addOption(std::string name, std::string value) {
            options[name] = value;
        }

        std::bitset<4> PeerConfig::getVersion() const {
            return *version;
        }

        unsigned short PeerConfig::getPort() const {
            return port;
        }

        std::string PeerConfig::getName() const {
            return name;
        }

        std::vector<shadapp::fs::Folder> PeerConfig::getFolders() const {
            return folders;
        }

        std::map<std::string, std::string> PeerConfig::getOptions() const {
            return options;
        }
    }
}
