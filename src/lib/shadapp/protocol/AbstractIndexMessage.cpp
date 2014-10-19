#include <shadapp/data/Serializer.h>
#include <shadapp/protocol/AbstractIndexMessage.h>

namespace shadapp {

    namespace protocol {

        AbstractIndexMessage::AbstractIndexMessage(
                Type type,
                std::bitset<4> version,
                std::string folder,
                std::vector<shadapp::fs::FileInfo> files)
        : AbstractMessage(version, type, false),
        folder(folder),
        files(files) {
        }
        
        AbstractIndexMessage::AbstractIndexMessage(unsigned char* bytes) : AbstractMessage(bytes) {
            unsigned int startIndex = 4;
            uint32_t size;
            size = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            folder = shadapp::data::Serializer::deserializeString(bytes, &startIndex, size);
            size = shadapp::data::Serializer::deserializeInt32(bytes, &startIndex);
            for (uint32_t i = 0; i < size; i++) {
                files.push_back(shadapp::fs::FileInfo::getFromBytes(bytes, &startIndex));
            }
        }

        void AbstractIndexMessage::addFile(shadapp::fs::FileInfo file) {
            files.push_back(file);
        }

        std::string AbstractIndexMessage::getFolder() const {
            return folder;
        }

        std::vector<shadapp::fs::FileInfo> AbstractIndexMessage::getFiles() const {
            return files;
        }
        
        unsigned char* AbstractIndexMessage::serialize(unsigned char* dest, unsigned int* size) const {
            if (AbstractMessage::serialize(dest, size) == nullptr) {
                return nullptr;
            }
            shadapp::data::Serializer::serializeInt32(dest, *size, folder.length(), size);
            shadapp::data::Serializer::serializeString(dest, *size, folder, size);
            shadapp::data::Serializer::serializeInt32(dest, *size, files.size(), size);
            for (auto f : files) {
                f.serialize(dest, size);
            }
            return dest;
        }
    }
}
