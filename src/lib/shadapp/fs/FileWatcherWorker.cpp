#include <iostream>

#include <Qt>
#include <QRegExp>

#include <shadapp/fs/FileWatcherWorker.h>

#include "shadapp/Logger.h"

namespace shadapp {

    namespace fs {

        FileWatcherWorker::FileWatcherWorker(std::string dirPath) {
            qRegisterMetaType<std::string>("std::string");
            folderFilter = QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::CaseSensitive;
            fileFilter = QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Readable | QDir::CaseSensitive;
            dir.setPath(QString(dirPath.c_str()));
            if (!dir.exists()) {
                throw std::runtime_error("Directory \"" + dirPath + "\" does not exist");
            }
        }

        FileWatcherWorker::~FileWatcherWorker() {
        }

        void FileWatcherWorker::process() {
            dir.refresh();
            QFileInfoList fileInfos = getFiles();
            std::map<std::string, long> files;
            // Detect new and modified files
            for (auto fileInfo : fileInfos) {
                std::string absPath = fileInfo.absoluteFilePath().toStdString();
                files[absPath] = getInode(absPath);
                if (previousFiles.find(absPath) == previousFiles.end()) {
                    emit newFile(absPath);
                } else if (lastScan < fileInfo.lastModified() || files[absPath] != previousFiles[absPath]) {
                    emit modifiedFile(absPath);
                }
            }
            // Detect removed files
            for (auto f : previousFiles) {
                if (files.find(f.first) == files.end()) {
                    emit deletedFile(f.first);
                }
            }
            previousFiles = files;
            lastScan = QDateTime::currentDateTime().addMSecs(1); // Force the incrementation
        }

        std::set<QString> FileWatcherWorker::getFolders() {
            std::set<QString> allFolders;
            allFolders.insert(dir.absolutePath());
            QDirIterator it(dir.absolutePath(), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::CaseSensitive, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                allFolders.insert(it.fileInfo().absoluteFilePath());
            }
            std::set<QString> foldersToScan;
            for (auto f : allFolders) {
                if (foldersToIgnore.find(QFileInfo(f).completeBaseName()) == foldersToIgnore.end() &&
                        //foldersToIgnore.find(subPath(QDir(f), dir, "")) == foldersToIgnore.end() &&
                        !isSubFolderOfIgnoredFolders(f)) {
                    foldersToScan.insert(f);
                } else {
                    Logger::debug("IGNORE FOLDER \"%s\"" ,f.toStdString().c_str());
                }
            }
            return foldersToScan;
        }

        QFileInfoList FileWatcherWorker::getFiles() {
            refreshPatternsToIgnore();
            QFileInfoList files;
            std::set<QString> folders = getFolders();
            for (auto folder : folders) {
                QDir currentDir(folder);
                QDirIterator it(folder, fileFilter, QDirIterator::NoIteratorFlags);
                while (it.hasNext()) {
                    it.next();
                    if (!mustBeIgnored(it, currentDir)) {
                        files.append(it.fileInfo());
                    }
                }
            }
            return files;
        }

        bool FileWatcherWorker::isSubFolderOfIgnoredFolders(const QString& folder) {
            QString baseDir = dir.absolutePath();
            for (auto relativeFolder : foldersToIgnore) {
                QString absPath = baseDir + QDir::separator() + relativeFolder;
                if (folder.startsWith(absPath)) {
                    return true;
                }
            }
            return false;
        }

        bool FileWatcherWorker::mustBeIgnored(const QDirIterator& it, const QDir& currentDir) {
            QString filename = it.fileName();
            QString relativePathFromBase = subPath(currentDir, dir, filename);
            for (auto pattern : patternsToIgnore) {
                if (currentDir.match(pattern, filename) || dir.match(pattern, relativePathFromBase)) {
                    Logger::debug("IGNORE FILE \"%s\"", it.filePath().toStdString().c_str());
                    return true;
                }
            }
            return false;
        }

        QString FileWatcherWorker::subPath(const QDir& currentDir, const QDir& baseDir, const QString& filename) {
            QString dirPath = currentDir.absolutePath().replace(baseDir.absolutePath(), "");
            dirPath = dirPath.remove(0, 1) + QDir::separator() + filename;
            int len = dirPath.length();
            return dirPath[len - 1] == QDir::separator() ? dirPath.remove(len - 1, 1) : dirPath;
        }

        void FileWatcherWorker::refreshPatternsToIgnore() {
            if (!(dir.exists(IGNOREFILE) && (lastIgnoreFileModif.isNull() || lastScan >= lastIgnoreFileModif))) {
                return;
            }
            QString ignoreFilePath = dir.absoluteFilePath(IGNOREFILE);
            QFile input(ignoreFilePath);
            if (!input.open(QIODevice::ReadOnly)) {
                Logger::error("Error while opening file");
                return;
            }
            //std::cout << "refresh ignore patterns" << std::endl;
            patternsToIgnore.clear();
            foldersToIgnore.clear();
            patternsToIgnore.insert(QString(IGNOREFILE));
            QTextStream in(&input);
            while (!in.atEnd()) {
                QString element = in.readLine().trimmed();
                if (element.length() > 0 && element[0] != '#') {
                    if (isDirectory(element)) {
                        if (element.endsWith(QDir::separator())) {
                            element.remove(element.length() - 1, 1);
                        }
                        foldersToIgnore.insert(element);
                        //std::cout << "FOLDER_IGN : " << element.toStdString() << std::endl;
                    } else {
                        patternsToIgnore.insert(element);
                        //std::cout << "PATTERN : " << element.toStdString() << std::endl;
                    }
                }
            }
            input.close();
        }

        bool FileWatcherWorker::isDirectory(const QString& name) {
            return dir.exists(name) && QFileInfo(dir.absoluteFilePath(name)).isDir();
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
