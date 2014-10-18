#ifndef CLUSTERCONFIGMESSAGE_H
#define	CLUSTERCONFIGMESSAGE_H

#include <map>
#include <string>
#include <vector>

#include <shadapp/fs/Folder.h>
#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        class ClusterConfigMessage : public AbstractMessage {
        private:
            std::string clientName;
            std::string clientVersion;
            std::vector<shadapp::fs::Folder> folders;
            std::map<std::string, std::string> options;

        public:
            explicit ClusterConfigMessage(
                    std::bitset<4> version,
                    std::string clientName,
                    std::string clientVersion,
                    std::vector<shadapp::fs::Folder> folders,
                    std::map<std::string,
                    std::string> options);
            explicit ClusterConfigMessage(unsigned char* bytes);

            std::string getClientName() const;
            std::string getClientVersion() const;
            std::vector<shadapp::fs::Folder> getFolders() const;
            std::map<std::string, std::string> getOptions() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;
        };
    }
}

#endif	// CLUSTERCONFIGMESSAGE_H
