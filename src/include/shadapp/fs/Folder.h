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
            explicit Folder(std::vector<uint8_t>& bytes);
            virtual ~Folder();
            
            bool operator==(const Folder &f1);

            void addDevice(Device* device);

            std::string getId() const;
            std::string getPath() const;
            std::vector<Device*> getDevices() const;

            std::vector<uint8_t> serialize() const override;
        };
    }
}

#endif	// FOLDER_H
