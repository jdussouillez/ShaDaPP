#ifndef INDEXMESSAGE_H
#define	INDEXMESSAGE_H

#include <bitset>

#include <shadapp/fs/FileInfo.h>
#include <shadapp/protocol/AbstractIndexMessage.h>

namespace shadapp {

    namespace protocol {

        class IndexMessage : public AbstractIndexMessage {
        public:
            explicit IndexMessage(
                    std::bitset<4> version,
                    std::string folder,
                    std::vector<shadapp::fs::FileInfo> files);
        };
    }
}

#endif	// INDEXMESSAGE_H