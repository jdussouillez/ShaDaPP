#include <iostream>

#include <shadapp/data/Serializer.h>
#include <shadapp/fs/Folder.h>
#include <algorithm>

typedef std::map<std::string, std::string> TStrFileInfoMap;

namespace shadapp {

    namespace fs {

        Folder::Folder(std::string id, std::string path) : id(id), path(path) {
            fileWatcher = new shadapp::fs::FileWatcher("test/Sync", 20);
        }

        Folder::Folder(std::string id) : Folder(id, "") {
        }

        Folder::Folder(std::vector<uint8_t>& bytes) {
            uint32_t idLength = shadapp::data::Serializer::deserializeInt32(bytes);
            id = shadapp::data::Serializer::deserializeString(bytes, idLength);
            uint32_t nbDevices = shadapp::data::Serializer::deserializeInt32(bytes);
            for (uint32_t i = 0; i < nbDevices; i++) {
                Device* d = new Device(bytes);
                addDevice(d);
                // TODO: Fix memory leak (= delete d)
            }
        }

        Folder::~Folder() {
            devices.clear();
        }

        bool Folder::operator==(const Folder& f1) {
            std::cout << "id this : " << this->getId() << "id :" << f1.getId() << std::endl;
            return this->getId().compare(f1.getId()) == 0;
        }

        void Folder::addDevice(Device* device) {
            devices.push_back(device);
        }

        void Folder::addFileInfo(FileInfo& fileInfo) {
            fileInfos.push_back(fileInfo);
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

        std::vector<shadapp::fs::FileInfo> Folder::getFileInfos() {
            return fileInfos;
        }

        void Folder::setPath(std::string path) {
            this->path = path;
        }

        std::vector<uint8_t> Folder::serialize() const {
            std::vector<uint8_t> bytes;
            shadapp::data::Serializer::serializeInt32(bytes, id.length());
            shadapp::data::Serializer::serializeString(bytes, id);
            shadapp::data::Serializer::serializeInt32(bytes, devices.size());
            for (auto d : devices) {
                std::vector<uint8_t> deviceBytes = d->serialize();
                bytes.insert(bytes.end(), deviceBytes.begin(), deviceBytes.end());
            }
            return bytes;
        }
    }
}
