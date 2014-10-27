#include <shadapp/fs/FileSplitter.h>

namespace shadapp {

    namespace fs {

        /*
         * 
         * Constructors & destructors
         * 
         */
        FileSplitter::FileSplitter(std::string filepath) : filepath(filepath) {
            stream.open(filepath.c_str(), std::ifstream::binary);
            stream.seekg(0, stream.end);
            size = stream.tellg();
            stream.seekg(0, stream.beg);
            nbBlocks = (size / MAX_BLOCK_SIZE) + (size % MAX_BLOCK_SIZE == 0 ? 0 : 1);
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
    }
}
