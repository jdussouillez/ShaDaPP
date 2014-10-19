#ifndef ABSTRACTINDEXMESSAGE_H
#define	ABSTRACTINDEXMESSAGE_H

#include <bitset>
#include <string>
#include <vector>

#include <shadapp/fs/FileInfo.h>
#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    namespace protocol {

        enum FileFlag : uint32_t {
            NO_PERM_INFO = 0x4000, // bit 17
            INVALID = 0x2000, // bit 18
            DELETED = 0x1000 // bit 19
        };

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
            explicit AbstractIndexMessage(unsigned char* bytes);

        public:
            void addFile(shadapp::fs::FileInfo file);

            std::string getFolder() const;
            std::vector<shadapp::fs::FileInfo> getFiles() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;
        };
    }
}

#endif	// ABSTRACTINDEXMESSAGE_H 
