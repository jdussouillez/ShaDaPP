#include <shadapp/fs/RequestedBlock.h>
#include <shadapp/fs/Folder.h>

#include "shadapp/Logger.h"

namespace shadapp {
    namespace fs {

        RequestedBlock::RequestedBlock(std::bitset<12> id,
                shadapp::fs::Folder* folder,
                shadapp::fs::FileInfo* fileInfo,
                long unsigned int* downloadBlockRemaining,
                std::string hash,
                uint64_t offset,
                uint32_t size) : id(id), folder(folder), fileInfo(fileInfo),
        downloadBlockRemaining(downloadBlockRemaining), hash(hash),
        offset(offset), size(size) {
        }

        void RequestedBlock::operator=(const RequestedBlock& block) {
            this->fileInfo = block.fileInfo;
            this->folder = block.folder;
            this->hash = block.hash;
            this->id = block.id;
            this->offset = block.offset;
            this->size = block.size;
        }

        std::string RequestedBlock::getFileName() {
            return fileInfo->getName();
        }

        shadapp::fs::FileInfo* RequestedBlock::getFileInfo() {
            return fileInfo;
        }

        long unsigned int* RequestedBlock::getDownloadBlockRemaining() const {
            return downloadBlockRemaining;
        }

        shadapp::fs::Folder* RequestedBlock::getFolder() {
            return folder;
        }

        std::string RequestedBlock::getHash() {
            return hash;
        }

        std::bitset<12> RequestedBlock::getId() {
            return id;
        }

        uint64_t RequestedBlock::getOffset() {
            return offset;
        }

        uint32_t RequestedBlock::getSize() {
            return size;
        }

        void RequestedBlock::setId(std::bitset<12> id) {
            this->id = id;
        }

        long unsigned int RequestedBlock::decreaseDownloadBlockRemaning() {
            (*downloadBlockRemaining)--;
            return *downloadBlockRemaining;
        }


    }

}
