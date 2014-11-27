#include <shadapp/protocol/ResponseMessage.h>
#include <shadapp/data/Serializer.h>
#include <shadapp/LocalPeer.h>

namespace shadapp {

    namespace protocol {

        ResponseMessage::ResponseMessage(std::bitset<4> version, std::string data)
        : Message(version, Type::RESPONSE, false),
        data(data.substr(0, MAX_BLOCK_SIZE)) {
        }

        ResponseMessage::ResponseMessage(std::vector<uint8_t>* bytes)
        : Message(bytes) {
            uint32_t length = shadapp::data::Serializer::deserializeInt32(bytes);
            data = shadapp::data::Serializer::deserializeString(bytes, length);
        }

        std::string ResponseMessage::getData() const {
            return data;
        }

        std::vector<uint8_t>* ResponseMessage::serialize(std::vector<uint8_t>* bytes) const {
            if (Message::serialize(bytes) == nullptr) {
                return nullptr;
            }
            shadapp::data::Serializer::serializeInt32(bytes, data.length());
            shadapp::data::Serializer::serializeString(bytes, data);
            return bytes;
        }
        
        void ResponseMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {

        }
    }
}
