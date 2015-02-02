#ifndef REQUESTEDBLOCK_H
#define	REQUESTEDBLOCK_H

#include <shadapp/fs/Folder.h>
#include <shadapp/fs/FileInfo.h>
#include <stdint.h>
#include <bitset>

namespace shadapp {

    namespace fs {
        class RequestedBlock{
        private:
            std::bitset<12> id;
            shadapp::fs::Folder *folder;
            shadapp::fs::FileInfo* fileInfo;
            long unsigned int* downloadBlockRemaining;
            std::string hash;
            uint64_t offset;
            uint32_t size;
            
            
        public:
            explicit RequestedBlock(std::bitset<12> id,
                shadapp::fs::Folder *folder,
                shadapp::fs::FileInfo* fileInfo,
                long unsigned int* downloadBlockRemaining,
                std::string hash,
                uint64_t offset,
                uint32_t size);
            
            void operator=(const RequestedBlock& block);
            
            std::bitset<12> getId();
            shadapp::fs::Folder* getFolder();
            std::string getFileName();
            shadapp::fs::FileInfo* getFileInfo();
            long unsigned int* getDownloadBlockRemaining() const;            
            std::string getHash();
            uint64_t getOffset();
            uint32_t getSize();
            
            void setId(std::bitset<12> id);
            
            long unsigned int decreaseDownloadBlockRemaning();
        };
    }
}

#endif	// REQUESTEDBLOCK_H 

