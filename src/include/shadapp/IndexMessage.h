#ifndef INDEXMESSAGE_H
#define	INDEXMESSAGE_H

#include <bitset>

#include <shadapp/AbstractIndexMessage.h>
#include <shadapp/FileInfo.h>

namespace shadapp {

    class IndexMessage : public AbstractIndexMessage {
    public:
        explicit IndexMessage(
                std::bitset<4> version,
                std::string folder,
                std::vector<FileInfo> files);
    };
}

#endif	// INDEXMESSAGE_H
