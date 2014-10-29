#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <string>

#include <shadapp/fs/FileWatcherWorker.h>

#include <QtCore/QThread>

// http://qt-project.org/wiki/QThreads_general_usage

namespace shadapp {

    namespace fs {

        class FileWatcher : public QThread {
            Q_OBJECT
        private:
            FileWatcherWorker* worker;
            unsigned int scanPeriod;

        public:
            explicit FileWatcher(std::string folder, unsigned int scanPeriod);
            virtual ~FileWatcher();

            void run() override;

        public slots:
            void newFile(std::string filePath);
            void deletedFile(std::string filePath);
            void modifiedFile(std::string filePath);
        };
    }
}

#endif	// FILEWATCHER_H
