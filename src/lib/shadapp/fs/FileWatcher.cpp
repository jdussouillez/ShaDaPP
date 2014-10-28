#include <iostream>

#include <shadapp/fs/FileWatcher.h>

namespace shadapp {

    namespace fs {

        FileWatcher::FileWatcher(std::string folder)
        : QThread(), folder(folder) {
            worker = new FileWatcherWorker(folder);
            QObject::connect(worker, SIGNAL(newFile(std::string)), this, SLOT(newFile(std::string)));
            QObject::connect(worker, SIGNAL(deletedFile(std::string)), this, SLOT(deletedFile(std::string)));
            QObject::connect(worker, SIGNAL(modifiedFile(std::string)), this, SLOT(modifiedFile(std::string)));
            QObject::connect(this, SIGNAL(started()), worker, SLOT(process()));
        }

        FileWatcher::~FileWatcher() {
            delete worker;
        }

        void FileWatcher::newFile(std::string file) {
            std::cout << "Created new file \"" << file << "\"" << std::endl;
        }

        void FileWatcher::deletedFile(std::string file) {
            std::cout << "Deleted file \"" << file << "\"" << std::endl;
        }

        void FileWatcher::modifiedFile(std::string file) {
            std::cout << "Modified file \"" << file << "\"" << std::endl;
        }
    }
}
