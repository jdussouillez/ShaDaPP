#ifndef INDEXUPDATEMESSAGE_H
#define	INDEXUPDATEMESSAGE_H

#include <bitset>

#include <shadapp/protocol/AbstractIndexMessage.h>

namespace shadapp {

    namespace protocol {

        class IndexUpdateMessage : public AbstractIndexMessage {
        public:
            explicit IndexUpdateMessage(
                    std::bitset<4> version,
                    std::string folder,
                    std::vector<shadapp::fs::FileInfo> files);
        };
    }
}

#endif	// INDEXUPDATEMESSAGE_H
