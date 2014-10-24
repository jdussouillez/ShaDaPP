#include <cstring>

#include <shadapp/data/Serializer.h>
#include <shadapp/fs/BlockInfo.h>

namespace shadapp {

    namespace fs {

        BlockInfo::BlockInfo(std::string hash, uint32_t size)
        : size(size), hash(hash) {
        }

        BlockInfo::BlockInfo(std::vector<uint8_t>* bytes) {
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            uint32_t hashLength = shadapp::data::Serializer::deserializeInt32(bytes);
            hash = shadapp::data::Serializer::deserializeString(bytes, hashLength);
        }

        std::string BlockInfo::getHash() const {
            return hash;
        }

        unsigned int BlockInfo::getSize() const {
            return size;
        }

        std::vector<uint8_t>* BlockInfo::serialize(std::vector<uint8_t>* bytes) const {
            shadapp::data::Serializer::serializeInt32(bytes, size);
            shadapp::data::Serializer::serializeInt32(bytes, hash.length());
            shadapp::data::Serializer::serializeString(bytes, hash);
            return bytes;
        }
    }
}
