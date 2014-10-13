#ifndef FILEINFO_H
#define	FILEINFO_H

#include <stdint.h>
#include <string>
#include <vector>

#include <shadapp/BlockInfo.h>

namespace shadapp {

    class FileInfo {
    private:
        std::string name;
        unsigned int flags;
        int64_t modified;
        uint64_t version;
        uint64_t localVer;
        std::vector<BlockInfo> blocks;

    public:
        explicit FileInfo(std::string name, uint64_t version, std::vector<BlockInfo> blocks);

        std::string getName() const;
        std::vector<BlockInfo> getBlocks() const;

    };
}

#endif	// FILEINFO_H 

