#ifndef PEERSHADAPP_H
#define	PEERSHADAPP_H


#include <map>
#include <QtNetwork/QTcpSocket>
#include <string>

#include <shadapp/protocol/AbstractMessage.h>
#include <shadapp/config/PeerConfig.h>

namespace shadapp {

    class Peer {
    private:
        shadapp::config::PeerConfig config;
        std::map<std::string, QTcpSocket*> sockets;

    public:
        bool connexionToPeer(std::string name,
            std::string id,
            unsigned int port);
        int send(std::string id, shadapp::protocol::AbstractMessage msg);
        int receive(std::string id, char* data);

    };

}

#endif	// PEERSHADAPP_H 
