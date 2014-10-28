#include <shadapp/fs/FileWatcherWorker.h>

namespace shadapp {

    namespace fs {

        FileWatcherWorker::FileWatcherWorker(std::string folder)
        : QObject(), folder(folder) {
            qRegisterMetaType<std::string>("std::string");
        }

        void FileWatcherWorker::process() {
            // TODO: scan folder and emit signals if a new/deleted/modified file is detected
        }
    }
}
