#include <shadapp/IndexMessage.h>

namespace shadapp {

    IndexMessage::IndexMessage(std::string folderID,
            std::vector<FileInfo> files)
    : folderID(folderID), files(files) {
    }

    void IndexMessage::addFile(FileInfo file) {
        files.push_back(file);
    }

    std::string IndexMessage::getId() const {
        return folderID;
    }

    std::vector<FileInfo> IndexMessage::getFiles() const {
        return files;
    }
}

