#ifndef CLIENTCONFIG_H
#define	CLIENTCONFIG_H

#include <map>
#include <string>
#include <vector>

#include <shadapp/Folder.h>

namespace shadapp {

    class ClientConfig {
    private:
        std::string version;
        unsigned short port;
        std::string name;
        std::vector<Folder> folders;
        std::map<std::string, std::string> options;

    public:
        void setVersion(std::string port);
        void setPort(unsigned short port);
        void setName(std::string name);
        void addFolder(Folder folder);
        void addOption(std::string name, std::string value);

        std::string getVersion() const;
        unsigned short getPort() const;
        std::string getName() const;
        std::vector<Folder> getFolders() const;
        std::map<std::string, std::string> getOptions() const;

    };
}

#endif	// CLIENTCONFIG_H