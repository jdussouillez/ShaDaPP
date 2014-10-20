#include <iostream>

#include <shadapp/data/Serializer.h>
#include <shadapp/fs/Folder.h>
#include <algorithm>

namespace shadapp {

    namespace fs {

        Folder::Folder(std::string id, std::string path)
        : id(id),
        path(path) {
        }

        Folder::Folder(std::string id)
        : Folder(id, "") {
        }

        Folder::~Folder() {
        }

        void Folder::addDevice(Device* device) {
            devices.push_back(device);
        }

        std::string Folder::getId() const {
            return id;
        }

        std::string Folder::getPath() const {
            return path;
        }

        std::vector<Device*> Folder::getDevices() const {
            return devices;
        }

        unsigned char* Folder::serialize(unsigned char* dest, unsigned int* size) const {
            shadapp::data::Serializer::serializeInt32(dest, *size, id.length(), size);
            shadapp::data::Serializer::serializeString(dest, *size, id, size);
            uint32_t nbDevices = devices.size();
            shadapp::data::Serializer::serializeInt32(dest, *size, nbDevices, size);
            for (auto device : devices) {
                device->serialize(dest, size);
            }
            return dest;
        }

        Folder Folder::getFromBytes(unsigned char* bytes, unsigned int* size) {
            uint32_t idLength = shadapp::data::Serializer::deserializeInt32(bytes, size);
            std::string id = shadapp::data::Serializer::deserializeString(bytes, size, idLength);
            Folder folder(id);
            uint32_t nbDevices = shadapp::data::Serializer::deserializeInt32(bytes, size);
            for (uint32_t i = 0; i < nbDevices; i++) {
                folder.addDevice(Device::getFromBytes(bytes, size));
            }
            return folder;
        }
    }
}
