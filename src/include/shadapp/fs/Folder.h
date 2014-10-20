#ifndef FOLDER_H
#define	FOLDER_H

#include <string>
#include <vector>

#include <shadapp/data/Serializable.h>
#include <shadapp/fs/Device.h>

namespace shadapp {

    namespace fs {

        class Folder : public shadapp::data::Serializable {
        private:
            std::string id;
            std::string path;
            std::vector<Device*> devices;

        public:
            explicit Folder(std::string id, std::string path);
            explicit Folder(std::string id);
            virtual ~Folder();

            void addDevice(Device* device);

            std::string getId() const;
            std::string getPath() const;
            std::vector<Device*> getDevices() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;

            static Folder getFromBytes(unsigned char* bytes, unsigned int* size);
        };
    }
}

#endif	// FOLDER_H
