#ifndef CLUSTERCONFIGMESSAGE_H
#define	CLUSTERCONFIGMESSAGE_H

#include <shadapp/Folder.h>
#include <map>
#include <string>
#include <vector>

namespace shadapp {

    class ClusterConfigMessage {
    private:
        std::string clientName;
        std::string clientVersion;
        std::vector<Folder> folders;
        std::map<std::string, std::string> options;

    public:
        ClusterConfigMessage(std::string clientName, std::string clientVersion, std::vector<Folder> folders, std::map<std::string, std::string> options);

        std::string getClientName() const;
        std::string getClientVersion() const;
        std::vector<Folder> getFolders() const;
        std::map<std::string, std::string> getOptions() const;
    };
}

#endif	// CLUSTERCONFIGMESSAGE_H
