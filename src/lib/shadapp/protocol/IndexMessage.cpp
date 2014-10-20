#include "shadapp/protocol/IndexMessage.h"

namespace shadapp {

    namespace protocol {

        IndexMessage::IndexMessage(
                std::bitset<4> version,
                std::string folder,
                std::vector<shadapp::fs::FileInfo> files)
        : AbstractIndexMessage(Type::INDEX, version, folder, files) {
        }

        IndexMessage::IndexMessage(unsigned char* bytes)
        : AbstractIndexMessage(bytes) {
        }
    }
}
