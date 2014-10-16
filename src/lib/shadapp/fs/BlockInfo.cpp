#include <shadapp/fs/BlockInfo.h>

namespace shadapp {

    namespace fs {

        BlockInfo::BlockInfo(unsigned int size = BLOCK_SIZE) : size(size) {
            std::memset(hash, 0, BLOCK_SIZE * sizeof (char));
        }

        const char* BlockInfo::getHash() const {
            return hash;
        }

        unsigned int BlockInfo::getSize() const {
            return size;
        }
    }
}
