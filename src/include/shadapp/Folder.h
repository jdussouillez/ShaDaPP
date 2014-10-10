#ifndef FOLDER_H
#define	FOLDER_H

#include <string>
#include <vector>

#include <shadapp/Device.h>

namespace shadapp {

    class Folder {
    private:
        std::string id;
        std::string path;
        std::vector<Device> devices;

    public:
        explicit Folder(std::string id, std::string path);

        void addDevice(Device device);

        std::string getId() const;
        std::string getPath() const;
        std::vector<Device> getDevices() const;
    };
}

#endif	// FOLDER_H
