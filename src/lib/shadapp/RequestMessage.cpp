#include <shadapp/RequestMessage.h>

namespace shadapp {

    RequestMessage::RequestMessage(std::string folder,
            std::string name,
            uint64_t offset,
            unsigned int size)
    : folder(folder),
    name(name),
    offset(offset),
    size(size) {
    }

    std::string RequestMessage::getFolder() const {
        return folder;
    }

    std::string RequestMessage::getName() const {
        return name;
    }

    uint64_t RequestMessage::getOffset() const {
        return offset;
    }

    unsigned int RequestMessage::getSize() const {
        return size;
    }

}
