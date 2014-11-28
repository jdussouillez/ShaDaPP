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

        AbstractIndexMessage::AbstractIndexMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            uint32_t size = shadapp::data::Serializer::deserializeInt32(bytes);
            folder = shadapp::data::Serializer::deserializeString(bytes, size);
            size = shadapp::data::Serializer::deserializeInt32(bytes);
            for (uint32_t i = 0; i < size; i++) {
                files.push_back(shadapp::fs::FileInfo(bytes));
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

        std::vector<uint8_t> AbstractIndexMessage::serialize() const {
            std::vector<uint8_t> bytes = AbstractMessage::serialize();
            shadapp::data::Serializer::serializeInt32(bytes, folder.length());
            shadapp::data::Serializer::serializeString(bytes, folder);
            shadapp::data::Serializer::serializeInt32(bytes, files.size());
            for (auto f : files) {
                std::vector<uint8_t> fileBytes = f.serialize();
                bytes.insert(bytes.end(), fileBytes.begin(), fileBytes.end());
            }
            return bytes;
        }
    }
}
