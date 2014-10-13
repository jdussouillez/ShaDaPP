#ifndef INDEXMESSAGE_H
#define	INDEXMESSAGE_H

#include <string>
#include <vector>

#include <shadapp/FileInfo.h>

namespace shadapp {

    class IndexMessage {
    private:
        std::string folderID;
        std::vector<FileInfo> files;

    public:
        explicit IndexMessage(std::string folderID, std::vector<FileInfo> files);

        void addFile(FileInfo file);

        std::string getId() const;
        std::vector<FileInfo> getFiles() const;
    };
}


#endif	// INDEXMESSAGE_H 
