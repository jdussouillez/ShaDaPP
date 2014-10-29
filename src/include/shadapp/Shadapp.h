
#include <shadapp/config/PeerConfig.h>
#include <shadapp/protocol/Message.h>

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

#include <shadapp/protocol/ClusterConfigMessage.h>
#include <shadapp/protocol/PingMessage.h>

namespace shadapp {

    class Shadapp : public QObject {
        Q_OBJECT
    private:
        shadapp::config::PeerConfig *config;
        QTcpServer *tcpServer;
        int connectedPeers;
        std::vector<QTcpSocket*> anonymousSocket;
    public:
        Shadapp(QObject *parent, std::string configFilePath);
        ~Shadapp();
        int start();
        unsigned int connection();
        int send(QTcpSocket *peer, shadapp::protocol::Message msg);
        std::vector<uint8_t>* receive(QTcpSocket* socket);
    
    
    
        
    public slots:
        void slotAcceptConnection();
        void slotSocketConnected();
        void slotSocketDisconnected();
        void slotSocketReceive();
        void slotPong(QTcpSocket *sock, shadapp::protocol::PingMessage ping);
        
    signals:
        void clusterConfig(shadapp::protocol::ClusterConfigMessage msg);
        void ping(QTcpSocket* sock,shadapp::protocol::PingMessage ping);
        
      
    };

}
