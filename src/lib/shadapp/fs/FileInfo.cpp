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

        FileInfo::FileInfo(std::vector<uint8_t>& bytes) {
            uint32_t nameLength = shadapp::data::Serializer::deserializeInt32(bytes);
            name = shadapp::data::Serializer::deserializeString(bytes, nameLength);
            flags = shadapp::data::Serializer::deserializeInt32(bytes);
            modified = shadapp::data::Serializer::deserializeInt64(bytes);
            version = shadapp::data::Serializer::deserializeInt64(bytes);
            localVersion = shadapp::data::Serializer::deserializeInt64(bytes);
            uint32_t nbBlocks = shadapp::data::Serializer::deserializeInt32(bytes);
            for (uint32_t i = 0; i < nbBlocks; i++) {
                blocks.push_back(shadapp::fs::BlockInfo(bytes));
            }
        }

        std::string FileInfo::getName() const {
            return name;
        }

        std::vector<BlockInfo> FileInfo::getBlocks() const {
            return blocks;
        }
        
        std::vector<uint8_t> FileInfo::serialize() const {
            std::vector<uint8_t> bytes;
            shadapp::data::Serializer::serializeInt32(bytes, name.length());
            shadapp::data::Serializer::serializeString(bytes, name);
            shadapp::data::Serializer::serializeInt32(bytes, flags);
            shadapp::data::Serializer::serializeInt64(bytes, modified);
            shadapp::data::Serializer::serializeInt64(bytes, version);
            shadapp::data::Serializer::serializeInt64(bytes, localVersion);
            shadapp::data::Serializer::serializeInt32(bytes, blocks.size());
            for (auto b : blocks) {
                std::vector<uint8_t> blockBytes = b.serialize();
                bytes.insert(bytes.end(), blockBytes.begin(), blockBytes.end());
            }
            return bytes;
        }
    }
}
