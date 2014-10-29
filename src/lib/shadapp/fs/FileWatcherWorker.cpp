#include <algorithm>
#include <iostream>

#include <shadapp/fs/FileWatcherWorker.h>

namespace shadapp {

    namespace fs {

        FileWatcherWorker::FileWatcherWorker(std::string dirPath) {
            qRegisterMetaType<std::string>("std::string");
            dir = new QDir(QString(dirPath.c_str()));
            if (!dir->exists()) {
                throw std::runtime_error("Directory \"" + dirPath + "\" does not exist");
            }
            dir->setFilter(QDir::Files
                    | QDir::NoDotAndDotDot
                    | QDir::NoSymLinks
                    | QDir::Readable
                    | QDir::CaseSensitive);
            // Put the filenames in a set
            QFileInfoList files = dir->entryInfoList();
            for (auto file : files) {
                previousFiles.insert(file.fileName().toStdString());
            }
            lastScan = QDateTime::currentDateTime();
        }

        FileWatcherWorker::~FileWatcherWorker() {
            delete dir;
        }

        void FileWatcherWorker::process() {
            dir->refresh();
            QFileInfoList fileInfos = dir->entryInfoList();
            //std::cout << lastScan.toString("dd/MM/yyyy hh:mm:ss:zzz").toStdString() << std::endl;
            std::set<std::string> files;
            for (auto fileInfo : fileInfos) {
                std::string filename = fileInfo.fileName().toStdString();
                files.insert(filename);
                if (previousFiles.find(filename) != previousFiles.end()) {
                    if (lastScan < fileInfo.lastModified()) {
                        emit modifiedFile(filename);
                    }
                } else {
                    previousFiles.insert(filename);
                    emit newFile(filename);
                }
            }
            for (auto f : previousFiles) {
                if (files.find(f) == files.end()) {
                    // f is in previous files but not in files -> it has been deleted
                    emit deletedFile(f);
                }
            }
            previousFiles = files;
            lastScan = QDateTime::currentDateTime().addMSecs(1); // Force the incrementation
            //std::cout << "NEW LAST SCAN " << lastScan.toString("dd/MM/yyyy hh:mm:ss:zzz").toStdString() << std::endl << std::endl;
        }
    }
}
