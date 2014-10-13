#include <shadapp/AbstractIndexMessage.h>

namespace shadapp {

    AbstractIndexMessage::AbstractIndexMessage(
            Type type,
            std::bitset<4> version,
            std::string folder,
            std::vector<FileInfo> files)
    : AbstractMessage(version, type, false),
    folder(folder),
    files(files) {
    }

    void AbstractIndexMessage::addFile(FileInfo file) {
        files.push_back(file);
    }

    std::string AbstractIndexMessage::getFolder() const {
        return folder;
    }

    std::vector<FileInfo> AbstractIndexMessage::getFiles() const {
        return files;
    }
}
