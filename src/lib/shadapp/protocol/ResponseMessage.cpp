#include <cstring>

#include <shadapp/protocol/ResponseMessage.h>

namespace shadapp {

    namespace protocol {

        ResponseMessage::ResponseMessage(std::bitset<4> version)
        : AbstractMessage(version, Type::RESPONSE, false) {
            std::memset(data, 0, BLOCK_SIZE * sizeof (char));
        }

        const char* ResponseMessage::getData() const {
            return data;
        }
    }
}
