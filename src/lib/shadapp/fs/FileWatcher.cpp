#include <iostream>

#include <shadapp/fs/FileWatcher.h>

namespace shadapp {

    namespace fs {

        FileWatcher::FileWatcher(std::string dirPath)
        : dirPath(dirPath) {
            worker = new FileWatcherWorker(dirPath);
            QObject::connect(worker, SIGNAL(newFile(std::string)), this, SLOT(newFile(std::string)));
            QObject::connect(worker, SIGNAL(deletedFile(std::string)), this, SLOT(deletedFile(std::string)));
            QObject::connect(worker, SIGNAL(modifiedFile(std::string)), this, SLOT(modifiedFile(std::string)));
        }

        FileWatcher::~FileWatcher() {
            delete worker;
        }

        void FileWatcher::run() {
            for (;;) {
                worker->process();
                QThread::sleep(30);
            }
        }

        void FileWatcher::newFile(std::string filePath) {
            std::cout << "Created new file \"" << filePath << "\"!\n";
        }

        void FileWatcher::deletedFile(std::string filePath) {
            std::cout << "Deleted file \"" << filePath << "\"!\n";
        }

        void FileWatcher::modifiedFile(std::string filePath) {
            std::cout << "Modified file \"" << filePath << "\"!\n";
        }
    }
}
