#ifndef CLIENTCONFIG_H
#define	CLIENTCONFIG_H

#include <bitset>
#include <map>
#include <string>
#include <vector>

#include <shadapp/fs/Folder.h>

namespace shadapp {

    namespace config {

        class ClientConfig {
        private:
            std::bitset<4>* version;
            unsigned short port;
            std::string name;
            std::vector<shadapp::fs::Folder> folders;
            std::map<std::string, std::string> options;

        public:
            ClientConfig();
            ~ClientConfig();

            void setVersion(std::string version);
            void setPort(unsigned short port);
            void setName(std::string name);
            void addFolder(shadapp::fs::Folder folder);
            void addOption(std::string name, std::string value);

            std::bitset<4> getVersion() const;
            unsigned short getPort() const;
            std::string getName() const;
            std::vector<shadapp::fs::Folder> getFolders() const;
            std::map<std::string, std::string> getOptions() const;

        };
    }
}

#endif	// CLIENTCONFIG_H
