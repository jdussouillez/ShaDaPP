#ifndef PEERCONFIG_H
#define	PEERCONFIG_H

#include <bitset>
#include <map>
#include <string>
#include <vector>

#include <shadapp/fs/Folder.h>

namespace shadapp {

    namespace config {

        class PeerConfig {
        private:
            std::bitset<4>* version;
            std::string id;
            unsigned short port;
            std::string name;
            std::string foldersPath;
            unsigned int scanPeriod;
            std::vector<shadapp::fs::Device*> devices;
            std::vector<shadapp::fs::Folder*> folders;
            std::map<std::string, std::string> options;

        public:
            PeerConfig();
            PeerConfig(const PeerConfig& other);
            ~PeerConfig();

            void setVersion(std::string version);
            void setID(std::string id);
            void setPort(unsigned short port);
            void setName(std::string name);
            void setFoldersPath(std::string path);
            void setScanPeriod(unsigned int scanPeriod);
            void addDevice(shadapp::fs::Device* device);
            void addFolder(shadapp::fs::Folder* folder);
            void addOption(std::string name, std::string value);

            std::bitset<4>* getVersion() const;
            std::string getID() const;
            unsigned short getPort() const;
            std::string getName() const;
            std::string getFoldersPath() const;
            unsigned int getScanPeriod() const;
            std::vector<shadapp::fs::Device*> getDevices() const;
            std::vector<shadapp::fs::Folder*> getFolders();
            std::map<std::string, std::string> getOptions() const;

            PeerConfig& operator=(const PeerConfig& other);
        };
    }
}

#endif	// PEERCONFIG_H
