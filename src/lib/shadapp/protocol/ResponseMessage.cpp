#include <shadapp/protocol/ResponseMessage.h>
#include <shadapp/data/Serializer.h>

namespace shadapp {

    namespace protocol {

        ResponseMessage::ResponseMessage(std::bitset<4> version, std::string data)
        : AbstractMessage(version, Type::RESPONSE, false),
        data(data.substr(0, MAX_BLOCK_SIZE)) {
        }

        ResponseMessage::ResponseMessage(unsigned char* bytes)
        : AbstractMessage(bytes) {
            unsigned int startIndex = 4;
            uint32_t length = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            data = shadapp::data::Serializer::deserializeString(bytes, &startIndex, length);
        }

        std::string ResponseMessage::getData() const {
            return data;
        }

        unsigned char* ResponseMessage::serialize(unsigned char* dest, unsigned int* size) const {
            if (AbstractMessage::serialize(dest, size) == nullptr) {
                return nullptr;
            }
            uint32_t length = data.size();
            shadapp::data::Serializer::serializeInt32(dest, *size, length, size);
            shadapp::data::Serializer::serializeString(dest, *size, data, size);
            return dest;
        }
    }
}
