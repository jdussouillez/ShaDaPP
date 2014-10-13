#ifndef CLUSTERCONFIGMESSAGE_H
#define	CLUSTERCONFIGMESSAGE_H

#include <map>
#include <string>
#include <vector>

#include <shadapp/Folder.h>
#include <shadapp/AbstractMessage.h>

namespace shadapp {

    class ClusterConfigMessage : public AbstractMessage {
    private:
        std::string clientName;
        std::string clientVersion;
        std::vector<Folder> folders;
        std::map<std::string, std::string> options;

    public:
        explicit ClusterConfigMessage(
                std::bitset<4> version,
                std::string clientName,
                std::string clientVersion,
                std::vector<Folder> folders,
                std::map<std::string,
                std::string> options);

        std::string getClientName() const;
        std::string getClientVersion() const;
        std::vector<Folder> getFolders() const;
        std::map<std::string, std::string> getOptions() const;
    };
}

#endif	// CLUSTERCONFIGMESSAGE_H
