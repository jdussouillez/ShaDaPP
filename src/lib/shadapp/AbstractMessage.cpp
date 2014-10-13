#include <shadapp/AbstractMessage.h>

namespace shadapp {

    AbstractMessage::AbstractMessage(std::bitset<4> version, Type type, bool compressed)
    : version(version), type(type), compressed(compressed) {
    }

    AbstractMessage::AbstractMessage(std::bitset<12> id, std::bitset<4> version, Type type, bool compressed)
    : AbstractMessage(version, type, compressed) {
        this->id = id;
    }

    std::bitset<4> AbstractMessage::getVersion() const {
        return version;
    }

    std::bitset<12> AbstractMessage::getId() const {
        return id;
    }

    Type AbstractMessage::getType() const {
        return type;
    }

    bool AbstractMessage::isCompressed() const {
        return compressed;
    }
}
