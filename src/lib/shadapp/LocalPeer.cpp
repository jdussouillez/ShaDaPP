#include <QtCore/QIODevice>
#include <QtCore/QString>

#include <shadapp/LocalPeer.h>
#include <shadapp/config/ConfigReader.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/Network.h>

namespace shadapp {

    LocalPeer::LocalPeer(shadapp::fs::Device* device, std::string configFilePath) {
        config = shadapp::config::ConfigReader::parse(std::string(configFilePath),
                "src/resources/shadapp/configSchema.xsd");
        network = new Network(0, this);
    }
    
    shadapp::config::PeerConfig * LocalPeer::getConfig() {
        return config;
    }
    
    void LocalPeer::start(){
        network->start();
    }
    

}

