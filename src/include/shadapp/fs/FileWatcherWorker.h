#ifndef FILEWATCHERWORKER_H
#define FILEWATCHERWORKER_H

#include <glob.h>
#include <map>
#include <set>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <shadapp/Core.h>

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QFile>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QTextStream>

namespace shadapp {

    namespace fs {

        class FileWatcherWorker : public QObject {
            Q_OBJECT
        private:
            QDir dir;
            QDateTime lastScan;
            QDateTime lastIgnoreFileModif;
            std::set<QString> patternsToIgnore;
            std::set<QString> foldersToIgnore;
            std::map<std::string, long> previousFiles; // filepath, inode
            QDir::Filters folderFilter;
            QDir::Filters fileFilter;

            QFileInfoList getFiles();
            std::set<QString> getFolders();
            void refreshPatternsToIgnore();

            /*
             * Difference between 2 paths : 
             * baseDir = /home/user/Sync
             * currentDir = /home/user/Sync/folder1
             * filename = file.txt
             * 
             * string returned : folder1/file.txt
             */
            bool mustBeIgnored(const QDirIterator& it, const QDir& currentDir);
            bool isSubFolderOfIgnoredFolders(const QString& folder);
            QString subPath(const QDir& currentDir, const QDir& baseDir, const QString& filename);
            inline bool isDirectory(const QString& name);

            static long getInode(std::string filename);

        public:
            explicit FileWatcherWorker(std::string dirPath);
            virtual ~FileWatcherWorker();

        signals:
            void newFile(std::string filePath);
            void deletedFile(std::string filePath);
            void modifiedFile(std::string filePath);

        public slots:
            void process();
        };
    }
}

#endif	// FILEWATCHERWORKER_H
