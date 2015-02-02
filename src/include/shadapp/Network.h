
#include <vector>

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

#include <shadapp/protocol/ClusterConfigMessage.h>
#include <shadapp/fs/Device.h>
#include <shadapp/protocol/AbstractMessage.h>
#include <shadapp/config/PeerConfig.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/fs/Folder.h>
//#include <shadapp/LocalPeer.h>

#include <shadapp/protocol/IndexMessage.h>

namespace shadapp {

    //class LocalPeer;
    
    class Network : public QObject {
        Q_OBJECT
    private:
        shadapp::config::PeerConfig *config;
        QTcpServer *tcpServer;
        int connectedPeers;
        shadapp::LocalPeer *lp;
        
        void initQtSignals(shadapp::fs::Device *device);
        
    public:
        Network(QObject *parent, shadapp::LocalPeer* lp);
        ~Network();
        int start();
        int send(QTcpSocket *peer, const shadapp::protocol::AbstractMessage& msg);
        int send2(QTcpSocket *peer, const shadapp::protocol::AbstractMessage& msg);
        std::vector<uint8_t>* receive(QTcpSocket* socket);
        
        LocalPeer* getLocalPeer();
        
    public slots:
        void slotAcceptConnection();
        void slotSocketConnected(shadapp::fs::Device *connectedDevice);
        void slotSocketDisconnected();
        void slotReceive(shadapp::fs::Device *device);
        void slotReceiveCCMfromNewPeer();
        
        void slotSendIndexUpdateMessage(shadapp::fs::Folder*, shadapp::fs::FileInfo*);
        
    };

}
