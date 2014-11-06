#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <shadapp/fs/FileWatcherWorker.h>

namespace shadapp {

    namespace fs {

        FileWatcherWorker::FileWatcherWorker(std::string dirPath) {
            qRegisterMetaType<std::string>("std::string");
            dir.setPath(QString(dirPath.c_str()));
            if (!dir.exists()) {
                throw std::runtime_error("Directory \"" + dirPath + "\" does not exist");
            }
            QFileInfoList files = getFiles(dir);
            for (auto file : files) {
                std::string absPath = file.absoluteFilePath().toStdString();
                previousFiles[absPath] = getInode(absPath);
            }
            lastScan = QDateTime::currentDateTime();
        }

        FileWatcherWorker::~FileWatcherWorker() {
        }

        void FileWatcherWorker::process() {
            dir.refresh();
            QFileInfoList fileInfos = getFiles(dir);
            std::map<std::string, long> files;
            for (auto fileInfo : fileInfos) {
                std::string filePath = fileInfo.absoluteFilePath().toStdString();
                files[filePath] = getInode(fileInfo.absoluteFilePath().toStdString());
                if (previousFiles.find(filePath) == previousFiles.end()) {
                    emit newFile(filePath);
                } else if (lastScan < fileInfo.lastModified() || previousFiles[filePath] != files[filePath]) {
                    emit modifiedFile(filePath);
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

        /*
         ***********************
         * Static methods
         ***********************
         */

        QFileInfoList FileWatcherWorker::getFiles(const QDir& base) {
            QFileInfoList files;
            QDir::Filters fileFilter = QDir::Files | QDir::NoSymLinks | QDir::Readable | QDir::CaseSensitive;
            QDirIterator it(base.absolutePath(), fileFilter, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                files.push_back(it.fileInfo());
            }
            return files;
        }

        long FileWatcherWorker::getInode(std::string filePath) {
            struct stat sb;
            if (stat(filePath.c_str(), &sb) == -1) {
                throw std::runtime_error("Could not get inode of file \"" + filePath + "\"");
            }
            return static_cast<long>(sb.st_ino);
        }
    }
}
