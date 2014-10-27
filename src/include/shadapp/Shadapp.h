#ifndef SHADAPP_H
#define	SHADAPP_H

#include <shadapp/protocol/AbstractMessage.h>
#include <shadapp/fs/Folder.h>
#include <shadapp/config/PeerConfig.h>

namespace shadapp {

    /**
     * class wich represents local peer
     */
    class Shadapp {
    private:
        shadapp::config::PeerConfig* config;

    public:
        Shadapp(std::string configFilePath);
        int start();
        int connection();
        int send(shadapp::protocol::AbstractMessage msg);
        int receive();
    };
}


#endif	// SHADAPP_H 

