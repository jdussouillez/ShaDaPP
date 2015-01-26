#ifndef FOLDER_H
#define	FOLDER_H

#include <string>
#include <vector>

#include <QObject>

#include <shadapp/data/Serializable.h>
#include <shadapp/fs/Device.h>
#include <shadapp/fs/FileWatcher.h>
#include <shadapp/fs/FileInfo.h>

namespace shadapp {

    namespace fs {

        class Folder : public QObject, public shadapp::data::Serializable {
            Q_OBJECT

        private:
            std::string id;
            std::string path;
            std::vector<Device*> devices;
            shadapp::fs::FileWatcher* fileWatcher;
            std::vector<shadapp::fs::FileInfo> fileInfos;
            bool onWatch;

        public:
            explicit Folder(std::string id, std::string path);
            explicit Folder(std::string id);
            explicit Folder(std::vector<uint8_t>& bytes);
            virtual ~Folder();

            bool operator==(const Folder &f1);

            void addDevice(Device* device);
            void addFileInfo(FileInfo& fileInfo);
            void replaceFileInfo(std::string name, FileInfo& fileInfo);

            std::string getId() const;
            std::string getPath() const;
            std::vector<Device*> getDevices() const;
            std::vector<shadapp::fs::FileInfo> getFileInfos();

            void setPath(std::string path);

            void createFileWatcher(std::string foldersPath);
            void startFileWatcher();
            void stopFileWatcher();

            std::vector<uint8_t> serialize() const override;
            
        signals:
            void signalFileModify(shadapp::fs::Folder*, shadapp::fs::FileInfo*);


        public slots:
            void slotFileWatcherCreate(std::string);
            void slotFileWatcherModify(std::string);
            void slotFileWatcherDelete(std::string);

        };
    }
}

#endif	// FOLDER_H
