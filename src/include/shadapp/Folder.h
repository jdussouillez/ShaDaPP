#ifndef FOLDER_H
#define	FOLDER_H

#include <vector>

namespace shadapp {

    class Folder {
    private:
        std::string id;
        std::string path;
        vector<Device> devices;

    public:
        explicit Folder(std::string id, std::string path);

        std::string getId() const;
        std::string getPath() const;
    };
}


#endif	// FOLDER_H

