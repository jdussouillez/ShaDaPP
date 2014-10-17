#include <shadapp/Core.h>
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
            int startIndex = 4; // Message header ends at byte 3
            folderLength = Serializer::deserializeInt32(bytes, startIndex);
            startIndex += sizeof (uint32_t);
            folder = Serializer::deserializeString(bytes, startIndex, folderLength);
            startIndex += folderLength;
            nameLength = Serializer::deserializeInt32(bytes, startIndex);
            startIndex += sizeof (uint32_t);
            name = Serializer::deserializeString(bytes, startIndex, nameLength);
            startIndex += nameLength;
            offset = Serializer::deserializeInt64(bytes, startIndex);
            startIndex += sizeof (uint64_t);
            size = Serializer::deserializeInt32(bytes, startIndex);
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
            if (dest == nullptr || size == nullptr) {
                return nullptr;
            }
            // Serialize the message header
            AbstractMessage::serialize(dest, size);
            // Serialize folder
            unsigned int foldernameSize = folder.size();
            Serializer::serializeInt32(dest, *size, foldernameSize, size);
            for (unsigned int i = 0; i < foldernameSize; i++) {
                dest[*size] = (unsigned char) folder.at(i);
                (*size)++;
            }
            // Serialize the name
            unsigned int filenameSize = name.size();
            Serializer::serializeInt32(dest, *size, filenameSize, size);
            for (unsigned int i = 0; i < filenameSize; i++) {
                dest[*size] = (unsigned char) name.at(i);
                (*size)++;
            }
            // Serialize the offset
            Serializer::serializeInt64(dest, *size, offset, size);
            // Serialize the size
            Serializer::serializeInt32(dest, *size, this->size, size);
            return dest;
        }
    }
}
