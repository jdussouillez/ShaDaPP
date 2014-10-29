#ifndef FILEWATCHERWORKER_H
#define FILEWATCHERWORKER_H

#include <set>
#include <string>

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QMetaType>
#include <QtCore/QObject>

namespace shadapp {

    namespace fs {

        class FileWatcherWorker : public QObject {
            Q_OBJECT
        private:
            QDir* dir;
            QDateTime lastScan;
            std::set<std::string> previousFiles;

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
