#include <bitset>
#include <iostream>


#include <shadapp/config/ConfigReader.h>
#include <shadapp/Core.h>
#include <shadapp/fs/Device.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/Shadapp.h>



namespace shadapp {

    Shadapp::Shadapp(QObject *parent, std::string configFilePath) : QObject(parent) {
        config = shadapp::config::ConfigReader::parse(std::string(configFilePath), "src/resources/shadapp/configSchema.xsd");
        QObject::connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    }

    int Shadapp::start() {
        unsigned int peerConnected = this->connection();
        std::cout << std::endl << "there are " << peerConnected << " peer connected" << std::endl;
        if (peerConnected != this->config->getDevices().size()) {
            //All peers are not connected
        }


        //        //test
        shadapp::protocol::PingMessage ping(*config->getVersion());
        int sendSize = this->send(config->getDevices().at(0), ping);
        int receiveSize = this->receive();
        std::cout << "\n\nsend : " << sendSize << " receive : " << receiveSize;
        //        //test end

        //1 clustter config message
        return 0;
    }

    unsigned int Shadapp::connection() {
        //launch server
        tcpServer.listen(QHostAddress::Any, quint16(config->getPort()));
        if (!tcpServer.isListening()) {
            std::cout << "Server does not listen...";
        } else {
            std::cout << "Listen socket is listening... On " << quint16(20001) << std::endl;
        }


        //connect to others peers
        QTcpSocket* socket = new QTcpSocket;

        int peerConnected = 0;
        for (auto &device : config->getDevices()) {
            std::cout << "\ntentative de connection a " << device->getAddress() << ":" << device->getPort();
            socket->connectToHost(QString(device->getAddress().c_str()),
                    device->getPort(), QIODevice::ReadWrite);
            device->setSocket(socket);
            if (socket->state() == QAbstractSocket::ConnectedState) {
                peerConnected++;
            }
        }

        return peerConnected;
    }

    int Shadapp::send(shadapp::fs::Device *device, shadapp::protocol::AbstractMessage msg) {
        std::vector<uint8_t> bytes;
        if (msg.serialize(&bytes) == nullptr) {
            // TODO: error
        }
        unsigned int sizeSend = device->getSocket()->write((const char*) &bytes.at(0), bytes.size());
        if (sizeSend != bytes.size()) {
            return 0;
        }
        return sizeSend;
    }

    int Shadapp::receive() {
        QTcpSocket* socket = config->getDevices().at(0)->getSocket();
        int sizeRead = socket->readBufferSize();
        //socket->readAll();
        //socket->read(data, sizeRead);
        return sizeRead;
    }

    void Shadapp::acceptConnection() {
        std::cout << "nouveau peer";
        QTcpSocket *clientConnection = tcpServer.nextPendingConnection();

    }

}
