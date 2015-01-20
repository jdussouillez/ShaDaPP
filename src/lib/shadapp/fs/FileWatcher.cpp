#include <iostream>

#include <shadapp/fs/FileWatcher.h>

namespace shadapp {

    namespace fs {

        FileWatcher::FileWatcher(std::string dirPath, unsigned int scanPeriod)
        : scanPeriod(scanPeriod) {
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
                QThread::sleep(scanPeriod);
            }
        }

        void FileWatcher::newFile(std::string filePath) {
            emit newFileSignal(filePath);
        }

        void FileWatcher::deletedFile(std::string filePath) {
            emit deletedFileSignal(filePath);
        }

        void FileWatcher::modifiedFile(std::string filePath) {
            emit modifiedFileSignal(filePath);
        }
    }
}
