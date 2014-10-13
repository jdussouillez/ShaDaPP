#include "shadapp/IndexMessage.h"

namespace shadapp {

    IndexMessage::IndexMessage(
            std::bitset<4> version,
            std::string folder,
            std::vector<FileInfo> files)
    : AbstractIndexMessage(Type::INDEX, version, folder, files) {
    }
}
