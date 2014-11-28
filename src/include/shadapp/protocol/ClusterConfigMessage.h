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
            explicit ClusterConfigMessage(std::vector<uint8_t>& bytes);

            std::string getClientName() const;
            std::string getClientVersion() const;
            std::vector<shadapp::fs::Folder> getFolders() const;
            std::map<std::string, std::string> getOptions() const;

            std::vector<uint8_t> serialize() const override;
        };
    }
}

#endif	// CLUSTERCONFIGMESSAGE_H
