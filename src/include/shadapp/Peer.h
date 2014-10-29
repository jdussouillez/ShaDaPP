#ifndef PEERSHADAPP_H
#define	PEERSHADAPP_H

#include <map>
#include <string>
#include <vector>

#include <QtNetwork/QTcpSocket>

#include <shadapp/fs/Folder.h>
#include <shadapp/protocol/Message.h>

namespace shadapp {

    class Peer {
    private:
        std::vector<shadapp::fs::Folder*> folders;
        QTcpSocket socket;
        shadapp::fs::Device* peerDevice;
        

    public:
        explicit Peer(shadapp::fs::Device* device);
        bool connect();
        int ping(std::bitset<4>* version);
        
    };

}

#endif	// PEERSHADAPP_H 
