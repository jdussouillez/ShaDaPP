#include <shadapp/fs/FileWatcherWorker.h>

namespace shadapp {

    namespace fs {

        FileWatcherWorker::FileWatcherWorker(std::string folder)
        : QObject(), folder(folder) {
        }

        void FileWatcherWorker::process() {
            for (;;) {
                emit newFile("foo");
                emit modifiedFile("bar");
                emit deletedFile("baz");
            }
        }
    }
}
