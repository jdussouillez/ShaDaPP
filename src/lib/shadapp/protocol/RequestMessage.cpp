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
        folder(folder.substr(0, FOLDERNAME_SIZE)),
        name(name.substr(0, FILENAME_SIZE)),
        offset(offset),
        size(size) {
        }

        RequestMessage::RequestMessage(unsigned char* bytes)
        : AbstractMessage(bytes) {
            uint32_t folderLength, nameLength;
            unsigned int startIndex = 4; // Message header ends at byte 3
            folderLength = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            folder = shadapp::data::Serializer::deserializeString(bytes, &startIndex, folderLength);
            nameLength = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            name = shadapp::data::Serializer::deserializeString(bytes, &startIndex, nameLength);
            offset = shadapp::data::Serializer::deserializeInt64(bytes, &startIndex);
            size = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
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

        unsigned char* RequestMessage::serialize(unsigned char* dest, unsigned int* size) const {
            if (AbstractMessage::serialize(dest, size) == nullptr) {
                return nullptr;
            }
            uint32_t foldernameSize = folder.size();
            shadapp::data::Serializer::serializeInt32(dest, *size, foldernameSize, size);
            for (unsigned int i = 0; i < foldernameSize; i++) {
                dest[*size] = (unsigned char) folder.at(i);
                (*size)++;
            }
            uint32_t filenameSize = name.size();
            shadapp::data::Serializer::serializeInt32(dest, *size, filenameSize, size);
            for (unsigned int i = 0; i < filenameSize; i++) {
                dest[*size] = (unsigned char) name.at(i);
                (*size)++;
            }
            shadapp::data::Serializer::serializeInt64(dest, *size, offset, size);
            shadapp::data::Serializer::serializeInt32(dest, *size, this->size, size);
            return dest;
        }
    }
}
