#include <shadapp/Core.h>
#include <shadapp/data/Serializer.h>
#include <shadapp/protocol/RequestMessage.h>

namespace shadapp {

    namespace protocol {

        RequestMessage::RequestMessage(
                std::bitset<4> version,
                std::string folder,
                std::string name,
                uint64_t offset,
                uint32_t size)
        : AbstractMessage(version, Type::REQUEST, false),
        folder(folder.substr(0, MAX_FOLDERNAME_SIZE)),
        name(name.substr(0, MAX_FILENAME_SIZE)),
        offset(offset),
        size(size) {
        }

        RequestMessage::RequestMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            uint32_t length;
            length = shadapp::data::Serializer::deserializeInt32(bytes);
            folder = shadapp::data::Serializer::deserializeString(bytes, length);
            length = shadapp::data::Serializer::deserializeInt32(bytes);
            name = shadapp::data::Serializer::deserializeString(bytes, length);
            offset = shadapp::data::Serializer::deserializeInt64(bytes);
            size = shadapp::data::Serializer::deserializeInt32(bytes);
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

        uint32_t RequestMessage::getSize() const {
            return size;
        }

        std::vector<uint8_t> RequestMessage::serialize() const {
            std::vector<uint8_t> bytes = AbstractMessage::serialize();
            shadapp::data::Serializer::serializeInt32(bytes, folder.length());
            shadapp::data::Serializer::serializeString(bytes, folder);
            shadapp::data::Serializer::serializeInt32(bytes, name.length());
            shadapp::data::Serializer::serializeString(bytes, name);
            shadapp::data::Serializer::serializeInt64(bytes, offset);
            shadapp::data::Serializer::serializeInt32(bytes, size);
            return bytes;
        }
    }
}
