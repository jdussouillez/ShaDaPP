#ifndef FILESPLITTER_H
#define FILESPLITTER_H

#include <fstream>
#include <string>

#include <shadapp/Core.h>

namespace shadapp {

    namespace fs {

        class FileSplitter {
        private:
            std::string filepath;
            std::ifstream stream;
            unsigned int size;
            unsigned int nbBlocks;
            
        public:
            explicit FileSplitter(std::string filepath);
            virtual ~FileSplitter();
            
            unsigned int getNbBlocks() const;
        };
    }
}

#endif	// FILESPLITTER_H
