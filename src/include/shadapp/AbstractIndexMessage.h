#ifndef ABSTRACTINDEXMESSAGE_H
#define	ABSTRACTINDEXMESSAGE_H

#include <bitset>
#include <string>
#include <vector>

#include <shadapp/FileInfo.h>
#include <shadapp/AbstractMessage.h>

namespace shadapp {

    class AbstractIndexMessage : public AbstractMessage {
    private:
        std::string folder;
        std::vector<FileInfo> files;

    protected:
        explicit AbstractIndexMessage(
                Type type,
                std::bitset<4> version,
                std::string folder,
                std::vector<FileInfo> files);

    public:
        void addFile(FileInfo file);

        std::string getFolder() const;
        std::vector<FileInfo> getFiles() const;
    };
}

#endif	// ABSTRACTINDEXMESSAGE_H 
