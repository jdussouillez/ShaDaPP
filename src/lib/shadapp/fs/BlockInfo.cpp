#include <cstring>

#include <shadapp/data/Serializer.h>
#include <shadapp/fs/BlockInfo.h>

namespace shadapp {

    namespace fs {

        BlockInfo::BlockInfo(std::string hash, uint32_t size = MAX_BLOCK_SIZE)
        : size(size),
        hash(hash.substr(0, MAX_BLOCK_SIZE)) {
        }

        std::string BlockInfo::getHash() const {
            return hash;
        }

        unsigned int BlockInfo::getSize() const {
            return size;
        }

        unsigned char* BlockInfo::serialize(unsigned char* dest, unsigned int* size) const {
            shadapp::data::Serializer::serializeInt32(dest, *size, this->size, size);
            shadapp::data::Serializer::serializeInt32(dest, *size, hash.length(), size);
            shadapp::data::Serializer::serializeString(dest, *size, hash, size);
            return dest;
        }

        BlockInfo BlockInfo::getFromBytes(unsigned char* bytes, unsigned int* size) {
            uint32_t blockSize = shadapp::data::Serializer::deserializeInt32(bytes, size);
            uint32_t hashLength = shadapp::data::Serializer::deserializeInt32(bytes, size);
            std::string hash = shadapp::data::Serializer::deserializeString(bytes, size, hashLength);
            return BlockInfo(hash, blockSize);
        }
    }
}
