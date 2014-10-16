#ifndef ABSTRACTINDEXMESSAGE_H
#define	ABSTRACTINDEXMESSAGE_H

#include <bitset>
#include <string>
#include <vector>

#include <shadapp/fs/FileInfo.h>
#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        class AbstractIndexMessage : public AbstractMessage {
        private:
            std::string folder;
            std::vector<shadapp::fs::FileInfo> files;

        protected:
            explicit AbstractIndexMessage(
                    Type type,
                    std::bitset<4> version,
                    std::string folder,
                    std::vector<shadapp::fs::FileInfo> files);

        public:
            void addFile(shadapp::fs::FileInfo file);

            std::string getFolder() const;
            std::vector<shadapp::fs::FileInfo> getFiles() const;
        };
    }
}

#endif	// ABSTRACTINDEXMESSAGE_H 
