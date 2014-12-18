#ifndef SAFEAPPLICATION_H
#define	SAFEAPPLICATION_H


#include <vector>

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QCoreApplication>



namespace shadapp {
    
    class SafeApplication : public QCoreApplication {
        //Q_OBJECT
        
    public:
        SafeApplication(int & argc, char ** argv);
        bool notify(QObject *receiver_, QEvent *event_);
    };

}


#endif	// SAFEAPPLICATION_H 

