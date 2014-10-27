#ifndef SHADAPP_H
#define	SHADAPP_H

#include <QObject>
#include <QTcpServer>


#include <shadapp/protocol/AbstractMessage.h>
#include <shadapp/fs/Folder.h>
#include <shadapp/fs/Device.h>
#include <shadapp/config/PeerConfig.h>

namespace shadapp {

    /**
     * class wich represents local peer
     */
    class Shadapp : public QObject {
    Q_OBJECT
    private:
        shadapp::config::PeerConfig* config;
        QTcpServer tcpServer;

    public:        
        Shadapp(QObject *parent, std::string configFilePath);
        int start();
        unsigned int connection();
        int send(shadapp::fs::Device *device, shadapp::protocol::AbstractMessage msg);
        int receive();
        
    public slots:
        void acceptConnection();
        
    };
}


#endif	// SHADAPP_H 
