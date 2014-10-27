
#include <shadapp/Shadapp.h>
#include <shadapp/config/ConfigReader.h>
#include <shadapp/Core.h>


namespace shadapp {

    Shadapp::Shadapp(std::string configFilePath) {
        config = shadapp::config::ConfigReader::parse(std::string(configFilePath), "src/resources/shadapp/configSchema.xsd");
    }

    int Shadapp::start() {
        int peerConnected = this->connection();
        //TODO test if number of connected peers is equals to the number of devices
//        if(peerConnected != this->config.){
//            
//        }
        
        
        
        //1 clustter config message
        return 0;
    }

    int Shadapp::connection() {
        QTcpSocket* socket = new QTcpSocket;
        int peerConnected = 0;
        for (auto &folder : config.getFolders()) {
            for (auto &device : folder.getDevices()) {
                socket->connectToHost(QString(device->getAddress().c_str()),
                        device->getPort(), QIODevice::ReadWrite);
                device->setSocket(socket);
                if (socket->state() == QAbstractSocket::ConnectedState) {
                    peerConnected++;
                }

            }
        }
        return peerConnected;
    }
    
//    int Shadapp::send(Device *device, AbstractMessage msg){
//        unsigned char data[MAX_MESSAGE_SIZE];
//        unsigned int size;
//        msg.serialize(data, &size);
//        unsigned int sizeSend = device->socket.write((const char*) data, size);
//        if (sizeSend != size) {
//            return 0;
//        }
//        return sizeSend;
//    }
}
