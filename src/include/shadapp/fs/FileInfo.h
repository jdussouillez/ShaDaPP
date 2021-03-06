#ifndef FILEINFO_H
#define	FILEINFO_H

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
            explicit FileInfo(std::string name,
                    uint64_t version,
                    std::vector<BlockInfo> blocks);
            explicit FileInfo(
                    std::string name,
                    uint32_t flags,
                    uint64_t modified,
                    uint64_t version,
                    uint64_t localVersion,
                    std::vector<BlockInfo> blocks);
            explicit FileInfo(std::vector<uint8_t>& bytes);

            bool operator==(const FileInfo &f1);
            
            std::string getName() const;
            std::vector<BlockInfo> getBlocks() const;
            uint64_t getVersion() const;
            uint64_t getLocalVersion() const;
            uint64_t getModified() const;
            
            bool isInvalid();
            bool isDeleted();
            bool isPermited();
            bool isSymbolicLink();
            bool isExistingTargetForSymbolicLink();
            
            void setValid(bool b);
            void setDeleted(bool b);
            void setPermit(bool b);
            void setSymbolicLink(bool b);
            void setExistingTargetForSymbolicLink(bool b);
            
            void increaseVersion(std::string absPath);
            void setVersion(uint64_t version);
            void setLocalVersion(uint64_t localVersion);
            void setModified(uint64_t modified);

            std::vector<uint8_t> serialize() const override;
        };
    }
}

#endif	// FILEINFO_H 
