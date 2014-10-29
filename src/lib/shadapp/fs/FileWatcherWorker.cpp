#include <algorithm>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
            QFileInfoList files = dir->entryInfoList();
            for (auto file : files) {
                previousFiles[file.fileName().toStdString()] = getInode(file.absoluteFilePath().toStdString());
            }
            lastScan = QDateTime::currentDateTime();
        }

        FileWatcherWorker::~FileWatcherWorker() {
            delete dir;
        }

        void FileWatcherWorker::process() {
            dir->refresh();
            QFileInfoList fileInfos = dir->entryInfoList();
            std::map<std::string, long> files;
            for (auto fileInfo : fileInfos) {
                std::string filename = fileInfo.fileName().toStdString();
                files[filename] = getInode(fileInfo.absoluteFilePath().toStdString());
                if (previousFiles.find(filename) == previousFiles.end()) {
                    emit newFile(filename);
                } else if (lastScan < fileInfo.lastModified() || previousFiles[filename] != files[filename]) {
                    emit modifiedFile(filename);
                }
            }
            for (auto f : previousFiles) {
                if (files.find(f.first) == files.end()) {
                    emit deletedFile(f.first);
                }
            }
            previousFiles = files;
            lastScan = QDateTime::currentDateTime().addMSecs(1); // Force the incrementation
        }

        long FileWatcherWorker::getInode(std::string filePath) {
            struct stat sb;
            if (stat(filePath.c_str(), &sb) == -1) {
                throw std::runtime_error("Could not get inode of file \"" + filePath + "\"");
            }
            return static_cast<long> (sb.st_ino);
        }
    }
}
