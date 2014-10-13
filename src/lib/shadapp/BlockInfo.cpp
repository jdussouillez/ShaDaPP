#include <shadapp/BlockInfo.h>

namespace shadapp {

    BlockInfo::BlockInfo(unsigned int size) : size(size) {
        hash = new char[size];
    }

    BlockInfo::~BlockInfo() {
        delete[] hash;
    }

    char* BlockInfo::getHash() const {
        return hash;
    }

    unsigned int BlockInfo::getSize() const {
        return size;
    }

}
