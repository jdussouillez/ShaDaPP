#include <cstdlib>

#include <shadapp/config/PeerConfig.h>

namespace shadapp {

    namespace config {

        PeerConfig::PeerConfig() : version(nullptr) {
        }

        PeerConfig::PeerConfig(const PeerConfig& other) {
            operator=(other);
        }

        PeerConfig::~PeerConfig() {
            delete version;
            for (auto &d : devices) {
                delete d;
            }
            devices.clear();
            for (auto &f : folders) {
                delete f;
            }
            folders.clear();
        }

        void PeerConfig::setVersion(std::string version) {
            int versionInteger = std::atoi(version.c_str());
            this->version = new std::bitset<4>(versionInteger);
        }

        void PeerConfig::setID(std::string id) {
            this->id = id;
        }

        void PeerConfig::setPort(unsigned short port) {
            this->port = port;
        }

        void PeerConfig::setName(std::string name) {
            this->name = name;
        }

        void PeerConfig::setFoldersPath(std::string path) {
            this->foldersPath = path;
        }

        void PeerConfig::setScanPeriod(unsigned int scanPeriod) {
            this->scanPeriod = scanPeriod;
        }

        void PeerConfig::addDevice(shadapp::fs::Device* device) {
            devices.push_back(device);
        }

        void PeerConfig::addFolder(shadapp::fs::Folder* folder) {
            folders.push_back(folder);
        }

        void PeerConfig::addOption(std::string name, std::string value) {
            options[name] = value;
        }

        std::bitset<4>* PeerConfig::getVersion() const {
            return version;
        }

        std::string PeerConfig::getID() const {
            return id;
        }

        unsigned short PeerConfig::getPort() const {
            return port;
        }

        std::string PeerConfig::getName() const {
            return name;
        }
        std::string PeerConfig::getFoldersPath() const {
            return foldersPath;
        }

        
        unsigned int PeerConfig::getScanPeriod() const {
            return scanPeriod;
        }

        std::vector<shadapp::fs::Device*> PeerConfig::getDevices() const {
            return devices;
        }

        std::vector<shadapp::fs::Folder*> PeerConfig::getFolders() {
            return folders;
        }

        std::map<std::string, std::string> PeerConfig::getOptions() const {
            return options;
        }

        PeerConfig& PeerConfig::operator=(const PeerConfig& other) {
            if (this != &other) {
                version = new std::bitset<4>(other.version);
                port = other.port;
                name = other.name;
                devices = other.devices;
                folders = other.folders;
                options = other.options;
            }
            return *this;
        }
    }
}
