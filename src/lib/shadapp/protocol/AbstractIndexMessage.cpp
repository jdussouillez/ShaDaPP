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

        void AbstractIndexMessage::addFile(shadapp::fs::FileInfo file) {
            files.push_back(file);
        }

        std::string AbstractIndexMessage::getFolder() const {
            return folder;
        }

        std::vector<shadapp::fs::FileInfo> AbstractIndexMessage::getFiles() const {
            return files;
        }
    }
}
