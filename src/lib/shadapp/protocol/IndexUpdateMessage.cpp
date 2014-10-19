#include <shadapp/protocol/IndexUpdateMessage.h>

namespace shadapp {

    namespace protocol {

        IndexUpdateMessage::IndexUpdateMessage(
                std::bitset<4> version,
                std::string folder,
                std::vector<shadapp::fs::FileInfo> files)
        : AbstractIndexMessage(Type::INDEX_UPDATE, version, folder, files) {
        }
        
        IndexUpdateMessage::IndexUpdateMessage(unsigned char* bytes)
        : AbstractIndexMessage(bytes) {
        }
    }
}
