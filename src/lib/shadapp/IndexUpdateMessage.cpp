#include <shadapp/IndexUpdateMessage.h>

namespace shadapp {

    IndexUpdateMessage::IndexUpdateMessage(
            std::bitset<4> version,
            std::string folder,
            std::vector<FileInfo> files)
    : AbstractIndexMessage(Type::INDEX_UPDATE, version, folder, files) {
    }
}
