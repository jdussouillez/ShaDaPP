#include <shadapp/data/Serializer.h>
#include <shadapp/fs/FileInfo.h>
#include <shadapp/fs/FileSplitter.h>
#include <shadapp/Core.h>
#include <shadapp/data/Hash.h>

#include <bitset>
#include <stdio.h>

#include "shadapp/fs/FileSplitter.h"
#include "shadapp/Logger.h"

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

        bool FileInfo::operator==(const FileInfo& f1) {
            Logger::debug("string this : %d", version);
            Logger::debug("string f1 : %s", f1.getName().c_str());
            return (name.compare(f1.getName()) == 0);
        }

        std::string FileInfo::getName() const {
            return name;
        }

        std::vector<BlockInfo> FileInfo::getBlocks() const {
            return blocks;
        }

        uint64_t FileInfo::getLocalVersion() const {
            return localVersion;
        }

        uint64_t FileInfo::getVersion() const {
            return version;
        }

        uint64_t FileInfo::getModified() const {
            return modified;
        }

        void FileInfo::setLocalVersion(uint64_t localVersion) {
            this->localVersion = localVersion;
        }

        void FileInfo::increaseVersion(std::string absPath) {
            version++;
            localVersion = version;
            std::bitset<32>newFlags(flags);
            newFlags[18] = 0;
            flags = newFlags.to_ullong();
            shadapp::fs::FileSplitter splitter(absPath);
            uint64_t offset = 0;
            blocks.clear();
            for (unsigned int i = 0; i < splitter.getNbBlocks(); i++) {
                std::vector<char> block = splitter.getBlock(offset, MAX_BLOCK_SIZE);
                std::string hash;
                shadapp::data::Hash256::hash(reinterpret_cast<uint8_t*> (&block[0]), block.size(), hash);
                shadapp::fs::BlockInfo blockInfo(hash, block.size());
                blocks.push_back(blockInfo);
                offset += MAX_BLOCK_SIZE;
            }
        }

        void FileInfo::setVersion(uint64_t version) {
            this->version = version;
        }

        void FileInfo::setModified(uint64_t modified) {
            this->modified = modified;
        }

        bool FileInfo::isDeleted() {
            return std::bitset<32>(flags)[19];
        }

        bool FileInfo::isExistingTargetForSymbolicLink() {
            return std::bitset<32>(flags)[15];
        }

        bool FileInfo::isInvalid() {
            return std::bitset<32>(flags)[18];
        }

        bool FileInfo::isPermited() {
            return std::bitset<32>(flags)[17];
        }

        bool FileInfo::isSymbolicLink() {
            return std::bitset<32>(flags)[16];
        }

        void FileInfo::setDeleted(bool b) {
            std::bitset<32> flagsTemp(flags);
            flagsTemp[19] = b;
            flags = flagsTemp.to_ullong();
        }

        void FileInfo::setExistingTargetForSymbolicLink(bool b) {
            Logger::debug("NOT IMPLEMENTED YET");
        }

        void FileInfo::setPermit(bool b) {
            Logger::debug("NOT IMPLEMENTED YET");
        }

        void FileInfo::setSymbolicLink(bool b) {
            Logger::debug("NOT IMPLEMENTED YET");
        }

        void FileInfo::setValid(bool b) {
            Logger::debug("NOT IMPLEMENTED YET");
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
