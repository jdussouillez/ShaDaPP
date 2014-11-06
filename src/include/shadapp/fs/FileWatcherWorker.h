#ifndef FILEWATCHERWORKER_H
#define FILEWATCHERWORKER_H

#include <set>
#include <map>
#include <string>

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QMetaType>
#include <QtCore/QObject>

namespace shadapp {

    namespace fs {

        class FileWatcherWorker : public QObject {
            Q_OBJECT
        private:
            QDir dir;
            QDateTime lastScan;
            std::map<std::string, long> previousFiles; // filename, inode
            
            // Returns all the files info contained in the folder "base" and its subfolders.
            static QFileInfoList getFiles(const QDir& base);
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
