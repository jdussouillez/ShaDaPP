#ifndef PEERSHADAPP_H
#define	PEERSHADAPP_H

#include <map>
#include <string>

#include <QtNetwork/QTcpSocket>

#include <shadapp/config/PeerConfig.h>
#include <shadapp/protocol/AbstractMessage.h>

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
