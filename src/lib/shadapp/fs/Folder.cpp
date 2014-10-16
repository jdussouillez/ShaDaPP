#include <shadapp/fs/Folder.h>

namespace shadapp {

    namespace fs {

        Folder::Folder(std::string id, std::string path)
        : id(id),
        path(path) {
        }

        void Folder::addDevice(Device device) {
            devices.push_back(device);
        }

        std::string Folder::getId() const {
            return id;
        }

        std::string Folder::getPath() const {
            return path;
        }

        std::vector<Device> Folder::getDevices() const {
            return devices;
        }
    }
}
