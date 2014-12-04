#include <shadapp/data/Serializer.h>
#include <shadapp/protocol/AbstractIndexMessage.h>
#include <shadapp/LocalPeer.h>

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
            // Set the message's length
            shadapp::data::Serializer::serializeInt32(bytes, bytes.size(), 4);
            return bytes;
        }

        void AbstractIndexMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
        }
    }
}
