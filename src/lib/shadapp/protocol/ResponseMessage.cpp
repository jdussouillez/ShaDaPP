#include <shadapp/protocol/ResponseMessage.h>
#include <shadapp/data/Serializer.h>

namespace shadapp {

    namespace protocol {

        ResponseMessage::ResponseMessage(std::bitset<4> version, std::string data)
        : AbstractMessage(version, Type::RESPONSE, false),
        data(data.substr(0, MAX_BLOCK_SIZE)) {
        }

        ResponseMessage::ResponseMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            uint32_t length = shadapp::data::Serializer::deserializeInt32(bytes);
            data = shadapp::data::Serializer::deserializeString(bytes, length);
        }

        std::string ResponseMessage::getData() const {
            return data;
        }

        std::vector<uint8_t> ResponseMessage::serialize() const {
            std::vector<uint8_t> bytes = AbstractMessage::serialize();
            shadapp::data::Serializer::serializeInt32(bytes, data.length());
            shadapp::data::Serializer::serializeString(bytes, data);
            // Set the message's length
            shadapp::data::Serializer::serializeInt32(bytes, bytes.size(), 4);
            return bytes;
        }
    }
}
