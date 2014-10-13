#include <shadapp/FileInfo.h>

namespace shadapp {

    FileInfo::FileInfo(std::string name,
            uint64_t version,
            std::vector<BlockInfo> blocks)
    : name(name), version(version),blocks(blocks) {
    }

    std::string FileInfo::getName() const {
        return name;
    }

    std::vector<BlockInfo> FileInfo::getBlocks() const {
        return blocks;
    }

}
