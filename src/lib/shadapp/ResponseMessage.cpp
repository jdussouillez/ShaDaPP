#include <cstring>

#include <shadapp/ResponseMessage.h>

namespace shadapp {

    ResponseMessage::ResponseMessage(std::bitset<4> version)
    : AbstractMessage(version, Type::RESPONSE, false) {
        std::memset(data, 0, BLOCK_SIZE * sizeof (char));
    }

    const char* ResponseMessage::getData() const {
        return data;
    }
}
