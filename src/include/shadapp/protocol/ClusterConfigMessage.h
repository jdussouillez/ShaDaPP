#ifndef CLUSTERCONFIGMESSAGE_H
#define	CLUSTERCONFIGMESSAGE_H

#include <map>
#include <string>
#include <vector>

#include <shadapp/fs/Folder.h>
#include <shadapp/protocol/Message.h>

namespace shadapp {

    namespace protocol {

        class ClusterConfigMessage : public Message {
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
            explicit ClusterConfigMessage(std::vector<uint8_t>* bytes);

            std::string getClientName() const;
            std::string getClientVersion() const;
            std::vector<shadapp::fs::Folder> getFolders() const;
            std::map<std::string, std::string> getOptions() const;

            std::vector<uint8_t>* serialize(std::vector<uint8_t>* bytes) const override;
        };
    }
}

#endif	// CLUSTERCONFIGMESSAGE_H
