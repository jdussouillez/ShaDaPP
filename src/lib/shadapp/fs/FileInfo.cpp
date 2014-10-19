#include <shadapp/data/Serializer.h>
#include <shadapp/fs/FileInfo.h>

namespace shadapp {

    namespace fs {

        FileInfo::FileInfo(std::string name,
                uint64_t version,
                std::vector<BlockInfo> blocks)
        : FileInfo(name, 0, 0, version, 0, blocks) {
        }

        FileInfo::FileInfo(std::string name,
                uint32_t flags,
                uint64_t modified,
                uint64_t version,
                uint64_t localVersion,
                std::vector<BlockInfo> blocks)
        : name(name),
        flags(flags),
        modified(modified),
        version(version),
        localVersion(localVersion),
        blocks(blocks) {
        }

        std::string FileInfo::getName() const {
            return name;
        }

        std::vector<BlockInfo> FileInfo::getBlocks() const {
            return blocks;
        }

        unsigned char* FileInfo::serialize(unsigned char* dest, unsigned int* size) const {
            shadapp::data::Serializer::serializeInt32(dest, *size, name.length(), size);
            shadapp::data::Serializer::serializeString(dest, *size, name, size);
            shadapp::data::Serializer::serializeInt32(dest, *size, flags, size);
            shadapp::data::Serializer::serializeInt64(dest, *size, modified, size);
            shadapp::data::Serializer::serializeInt64(dest, *size, version, size);
            shadapp::data::Serializer::serializeInt64(dest, *size, localVersion, size);
            shadapp::data::Serializer::serializeInt32(dest, *size, blocks.size(), size);
            for (auto b : blocks) {
                b.serialize(dest, size);
            }
            return dest;
        }

        FileInfo FileInfo::getFromBytes(unsigned char* bytes, unsigned int* size) {
            uint32_t nameLen;
            nameLen = shadapp::data::Serializer::deserializeInt32(bytes, size);
            std::string name = shadapp::data::Serializer::deserializeString(bytes, size, nameLen);
            uint32_t flags = shadapp::data::Serializer::deserializeInt32(bytes, size);
            uint64_t modified = shadapp::data::Serializer::deserializeInt64(bytes, size);
            uint64_t version = shadapp::data::Serializer::deserializeInt64(bytes, size);
            uint64_t localVersion = shadapp::data::Serializer::deserializeInt64(bytes, size);
            uint32_t nbBlocks = shadapp::data::Serializer::deserializeInt32(bytes, size);
            std::vector<shadapp::fs::BlockInfo> blocks;
            for (uint32_t i = 0; i < nbBlocks; i++) {
                blocks.push_back(shadapp::fs::BlockInfo::getFromBytes(bytes, size));
            }
            return FileInfo(name, flags, modified, version, localVersion, blocks);
        }
    }
}
