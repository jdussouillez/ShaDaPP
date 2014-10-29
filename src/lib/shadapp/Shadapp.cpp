#include <bitset>
#include <iostream>

#include <chrono>
#include <thread>

#include <shadapp/config/ConfigReader.h>
#include <shadapp/Core.h>
#include <shadapp/fs/Device.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/Shadapp.h>

#include "shadapp/protocol/PongMessage.h"
#include "shadapp/protocol/ClusterConfigMessage.h"


namespace shadapp {

    Shadapp::Shadapp(QObject *parent, std::string configFilePath) : QObject(parent) {
        connectedPeers = 0;
        config = shadapp::config::ConfigReader::parse(std::string(configFilePath), "src/resources/shadapp/configSchema.xsd");
        this->tcpServer = new QTcpServer(0);
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotAcceptConnection()));
        connect(this, SIGNAL(ping(QTcpSocket*, shadapp::protocol::PingMessage)), this, SLOT(slotPong(QTcpSocket*, shadapp::protocol::PingMessage)));
    }

    Shadapp::~Shadapp() {
        for (auto &device : config->getDevices()) {
            delete device->getSocket();
        }
    }

    int Shadapp::start() {

        this->connection();
        //std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        //std::cout << std::endl << "there are " << this->connectedPeers << " /" << this->config->getDevices().size() << " peer connected" << std::endl;

        //TODO remove PING
        if (this->config->getID().compare("4568976546554") == 0) {
            std::cout << "debug";
            shadapp::protocol::PingMessage ping(*config->getVersion());
            int sendSize = this->send(config->getDevices().at(1)->getSocket(), ping);
            std::cout << std::endl << "send" << sendSize << std::endl;
        }
        return 0;
    }

    unsigned int Shadapp::connection() {
        //launch server

        tcpServer->listen(QHostAddress::Any, quint16(config->getPort()));
        if (!tcpServer->isListening()) {
            std::cout << "Server does not listen...";
        } else {
            std::cout << "Listen socket is listening... On " << quint16(config->getPort()) << std::endl;
        }


        for (auto &device : config->getDevices()) {
            device->setSocket(new QTcpSocket(this));
            connect(device->getSocket(), SIGNAL(connected()), this, SLOT(slotSocketConnected()));
            connect(device->getSocket(), SIGNAL(disconnected()), this, SLOT(slotSocketDisconnected()));
            connect(device->getSocket(), SIGNAL(readyRead()), this, SLOT(slotSocketReceive()));
            device->getSocket()->connectToHost(QString(device->getAddress().c_str()), quint16(device->getPort()));
        }
    }

    int Shadapp::send(QTcpSocket *peer, shadapp::protocol::Message msg) {
        std::vector<uint8_t> bytes;
        if (msg.serialize(&bytes) == nullptr) {
            // TODO: error
        }
        unsigned int sizeSend = peer->write((const char*) &bytes.at(0), bytes.size());
        if (sizeSend != bytes.size()) {
            return 0;
        }
        return sizeSend;
    }

    std::vector<uint8_t>* Shadapp::receive(QTcpSocket* socket) {
        int sizeRead = socket->bytesAvailable();
        std::vector<uint8_t> *data = new std::vector<uint8_t>(sizeRead);
        socket->read((char*) &data->at(0), sizeRead);
        std::cout << "data size : " << data->size() << std::endl;
        return data;
    }

    void Shadapp::slotAcceptConnection() {
        std::cout << "nouveau peer" << std::endl;
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        connect(socket, SIGNAL(connected()), this, SLOT(slotSocketConnected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(slotSocketDisconnected()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(slotSocketReceive()));
        anonymousSocket.push_back(socket);
    }

    void Shadapp::slotSocketConnected() {
        connectedPeers++;
    }

    void Shadapp::slotSocketDisconnected() {
        connectedPeers--;
    }

    void Shadapp::slotSocketReceive() {
        QTcpSocket* socket = dynamic_cast<QTcpSocket*> (sender());
        //remove
        std::vector<uint8_t>* data = receive(socket);
        std::cout << "data size : " << data->size() << std::endl;
        shadapp::protocol::Message msg(data, false);
        
        
        switch (msg.getType()) {
            case shadapp::protocol::Type::CLUSTER_CONFIG:
   //             emit clusterConfig(((shadapp::protocol::ClusterConfigMessage)msg));
   
                break;
            case shadapp::protocol::Type::INDEX:
                break;
            case shadapp::protocol::Type::REQUEST:
                break;
            case shadapp::protocol::Type::RESPONSE:
                break;
            case shadapp::protocol::Type::PING:{
                shadapp::protocol::PingMessage pingMsg(data);
                emit ping(socket, pingMsg);
            }
                break;
            case shadapp::protocol::Type::PONG:
                std::cout<<"PONG"<<std::endl;
                break;
            case shadapp::protocol::Type::INDEX_UPDATE:
                break;
            case shadapp::protocol::Type::CLOSE:
                break;
        }
        delete data;
    }
    
    void Shadapp::slotPong(QTcpSocket *sock, shadapp::protocol::PingMessage ping){
        std::cout<<"receive ping"<<std::endl;
        shadapp::protocol::PongMessage pong(ping.getVersion(), ping);
        this->send(sock, pong);
    }

}
