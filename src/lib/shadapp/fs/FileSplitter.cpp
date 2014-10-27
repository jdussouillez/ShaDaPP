#include <shadapp/fs/FileSplitter.h>

namespace shadapp {

    namespace fs {

        /*
         * 
         * Constructors & destructors
         * 
         */
        FileSplitter::FileSplitter(std::string filepath) : filepath(filepath) {
            stream.open(filepath.c_str());
            stream.seekg(0, stream.end);
            fileSize = stream.tellg();
            stream.seekg(0, stream.beg);
            nbBlocks = (fileSize / MAX_BLOCK_SIZE) + (fileSize % MAX_BLOCK_SIZE == 0 ? 0 : 1);
        }

        FileSplitter::~FileSplitter() {
            stream.close();
        }

        /*
         * 
         * Getters
         * 
         */
        unsigned int FileSplitter::getNbBlocks() const {
            return nbBlocks;
        }

        std::vector<char> FileSplitter::getBlock(uint64_t offset, uint32_t size) {
            if (offset + size > fileSize) {
                size = fileSize - offset;
            }
            std::vector<char> buffer(size);
            stream.seekg(offset, stream.beg);
            stream.read(&buffer[0], size);
            return buffer;
        }
    }
}
