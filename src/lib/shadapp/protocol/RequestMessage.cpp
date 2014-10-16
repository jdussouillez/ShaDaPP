#include <shadapp/protocol/RequestMessage.h>

namespace shadapp {

    namespace protocol {

        RequestMessage::RequestMessage(
                std::bitset<4> version,
                std::string folder,
                std::string name,
                uint64_t offset,
                unsigned int size)
        : AbstractMessage(version, Type::REQUEST, false),
        folder(folder),
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
}
