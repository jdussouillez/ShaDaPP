#include <shadapp/Folder.h>

namespace shadapp {

    Folder::Folder(std::string id, std::string path) : id(id), path(path) {
    }

    std::string Folder::getId() const {
        return id;
    }

    std::string Folder::getPath() const {
        return path;
    }
}
