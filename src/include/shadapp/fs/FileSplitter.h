#ifndef FILESPLITTER_H
#define FILESPLITTER_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <shadapp/Core.h>

namespace shadapp {

    namespace fs {

        class FileSplitter {
        private:
            std::string filepath;
            std::ifstream stream;
            unsigned int fileSize;
            unsigned int nbBlocks;
            
        public:
            explicit FileSplitter(std::string filepath);
            virtual ~FileSplitter();
            
            unsigned int getNbBlocks() const;
            std::vector<char> getBlock(uint64_t offset, uint32_t size);
        };
    }
}

#endif	// FILESPLITTER_H
