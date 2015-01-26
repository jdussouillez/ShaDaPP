#ifndef LOCALPEER_H
#define	LOCALPEER_H

#include <map>
#include <string>
#include <vector>
#include <bitset>

#include <QtNetwork/QTcpSocket>

#include <shadapp/fs/Folder.h>
#include <shadapp/config/PeerConfig.h>

#include <shadapp/fs/FileInfo.h>
#include <shadapp/fs/RequestedBlock.h>


//#include <shadapp/Network.h>

namespace shadapp {

    class Network;
    
    class LocalPeer {
    private:
        shadapp::config::PeerConfig *config;
        shadapp::Network *network;
        std::vector<shadapp::fs::FileInfo> fileToDownload;
        std::bitset<12> messageId;
        std::map<unsigned long, shadapp::fs::RequestedBlock*> requestedBlocks;
        
    public:
        explicit LocalPeer(shadapp::fs::Device* device, std::string configFilePath);
        ~LocalPeer();
        
        shadapp::config::PeerConfig* getConfig();
        shadapp::Network* getNetwork() const;
        std::map<unsigned long, shadapp::fs::RequestedBlock*> getRequestedBlocks();
        
        void start();        
        void addRequestedBlock(shadapp::fs::RequestedBlock* block);
        std::bitset<12> generateMessageId();
        
        void removeRequestedBlock(uint64_t id);
        
        void sendPingMessage(shadapp::fs::Device *device);        
        void sendAllIndexMessage(shadapp::fs::Device *device, std::vector<shadapp::fs::Folder*> folders);
        void sendIndexMessage(shadapp::fs::Device *device, shadapp::fs::Folder* folders);
    };

}

#endif	// LOCALPEER_H 
