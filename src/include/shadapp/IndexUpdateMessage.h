#ifndef INDEXUPDATEMESSAGE_H
#define	INDEXUPDATEMESSAGE_H

#include <bitset>

#include <shadapp/AbstractMessage.h>
#include <shadapp/AbstractIndexMessage.h>

namespace shadapp {

    class IndexUpdateMessage : public AbstractIndexMessage {
    public:
        explicit IndexUpdateMessage(
                std::bitset<4> version,
                std::string folder,
                std::vector<FileInfo> files);
    };
}

#endif	// INDEXUPDATEMESSAGE_H
