#ifndef FILEINFO_H
#define	FILEINFO_H

#include <stdint.h>
#include <string>
#include <vector>

#include <shadapp/data/Serializable.h>
#include <shadapp/fs/BlockInfo.h>

namespace shadapp {

    namespace fs {

        class FileInfo : public shadapp::data::Serializable {
        private:
            std::string name;
            uint32_t flags;
            uint64_t modified;
            uint64_t version;
            uint64_t localVersion;
            std::vector<BlockInfo> blocks;

        public:
            explicit FileInfo(std::string name, uint64_t version, std::vector<BlockInfo> blocks);
            explicit FileInfo(
                    std::string name,
                    uint32_t flags,
                    uint64_t modified,
                    uint64_t version,
                    uint64_t localVersion,
                    std::vector<BlockInfo> blocks);

            std::string getName() const;
            std::vector<BlockInfo> getBlocks() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;

            static FileInfo getFromBytes(unsigned char* bytes, unsigned int* size);
        };
    }
}

#endif	// FILEINFO_H 
