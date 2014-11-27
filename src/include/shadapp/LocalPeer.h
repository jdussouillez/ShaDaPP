#ifndef LOCALPEER_H
#define	LOCALPEER_H

#include <map>
#include <string>
#include <vector>

#include <QtNetwork/QTcpSocket>

#include <shadapp/fs/Folder.h>
#include <shadapp/config/PeerConfig.h>

//#include <shadapp/Network.h>

namespace shadapp {

    class Network;
    
    class LocalPeer {
    private:
        shadapp::config::PeerConfig *config;
        shadapp::Network *network;
    public:
        explicit LocalPeer(shadapp::fs::Device* device, std::string configFilePath);
        
        shadapp::config::PeerConfig* getConfig();
        
        
        void start();
    };

}

#endif	// LOCALPEER_H 
