#include <shadapp/SafeApplication.h>
#include <iostream>

namespace shadapp {
    
    SafeApplication::SafeApplication(int& argc, char** argv):QCoreApplication(argc,argv) {

    }

    
    bool SafeApplication::notify(QObject *receiver_, QEvent *event_) {
        try {
            return QCoreApplication::notify(receiver_, event_);
        } catch (std::exception &ex) {
            std::cerr << "std::exception was caught : " << ex.what() << std::endl;
        }
        return false;
    }
}