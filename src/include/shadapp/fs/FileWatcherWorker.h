#ifndef FILEWATCHERWORKER_H
#define FILEWATCHERWORKER_H

#include <string>

#include <QtCore/QObject>

namespace shadapp {

    namespace fs {

        class FileWatcherWorker : public QObject {
            Q_OBJECT
        private:
            std::string folder;

        public:
            explicit FileWatcherWorker(std::string folder);

        signals:
            void newFile(std::string file);
            void deletedFile(std::string file);
            void modifiedFile(std::string file);

        public slots:
            void process();
        };
    }
}

#endif	// FILEWATCHERWORKER_H
