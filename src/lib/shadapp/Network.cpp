#include <bitset>
#include <iostream>

#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>

#include <shadapp/config/ConfigReader.h>
#include <shadapp/Core.h>
#include <shadapp/fs/Device.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/Network.h>

#include <shadapp/protocol/PongMessage.h>
#include <shadapp/protocol/ClusterConfigMessage.h>

#include "shadapp/protocol/RequestMessage.h"
#include "shadapp/protocol/ResponseMessage.h"
#include "shadapp/protocol/IndexUpdateMessage.h"
#include "shadapp/protocol/CloseMessage.h"

//AutoConnection
//UniqueConnection
//BlockingQueuedConnection
//QueuedConnection
#define connectType Qt::QueuedConnection


namespace shadapp {

    Network::Network(QObject *parent, shadapp::LocalPeer* lp) : QObject(parent) {
        connectedPeers = 0;
        this->lp = lp;
        this->tcpServer = new QTcpServer(0);
        //Launch when another peer connect to this local peer
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotAcceptConnection()));
    }

    Network::~Network() {
        for (auto &device : lp->getConfig()->getDevices()) {
            delete device->getSocket();
        }
    }

    LocalPeer* Network::getLocalPeer() {
        return lp;
    }

    void Network::initQtSignals(shadapp::fs::Device *device) {
        std::cout << "init socket" << std::endl;
        //La socket lance le slot de la device
        connect(device->getSocket(), SIGNAL(connected()),
                device, SLOT(slotDeviceConnected()));
        //la device renvoie son adresse a network.cpp
        connect(device, SIGNAL(signalConnected(shadapp::fs::Device*)),
                this, SLOT(slotSocketConnected(shadapp::fs::Device*)));

        connect(device->getSocket(), SIGNAL(readyRead()),
                device, SLOT(slotReceive()), connectType);
        connect(device, SIGNAL(signalReceive(shadapp::fs::Device*)),
                this, SLOT(slotReceive(shadapp::fs::Device*)), connectType);

        connect(device->getSocket(), SIGNAL(disconnected()),
                this, SLOT(slotSocketDisconnected()));
    }

    int Network::start() {

        tcpServer->listen(QHostAddress::Any, quint16(lp->getConfig()->getPort()));
        if (!tcpServer->isListening()) {
            std::cout << "Server does not listen...";
        } else {
            std::cout << "Listen socket is listening... On "
                    << quint16(lp->getConfig()->getPort()) << std::endl;
        }
        for (auto &device : lp->getConfig()->getDevices()) {
            initQtSignals(device);
            device->getSocket()->connectToHost(QString(device->getAddress().c_str()),
                    quint16(device->getPort()));
        }
        return 0;
    }

    int Network::send(QTcpSocket *peer, const shadapp::protocol::AbstractMessage& msg) {
        std::vector<uint8_t> bytes = msg.serialize();
        shadapp::Logger::debug("SEND");
        std::cout << "size : " << bytes.size() << std::endl;
        unsigned int sizeSend = peer->write((const char*) &bytes.at(0), bytes.size());
        if (sizeSend != bytes.size()) {
            return 0;
        }
        return sizeSend;
    }
    
    std::vector<uint8_t>* Network::receive(QTcpSocket* socket) {
        QByteArray thirdByte = socket->peek(8);
        std::vector<uint8_t> lengthBytes(thirdByte.begin()+4,thirdByte.end());
        int length = shadapp::data::Serializer::deserializeInt32(lengthBytes);
        shadapp::Logger::debug("RECEIVE");
        std::cout << "size : " << length << std::endl;
        std::vector<uint8_t> *data = new std::vector<uint8_t>(length);
        socket->read((char*) &data->at(0), length);
        std::cout << "socket error " << socket->state() << std::endl;
        return data;
    }

    void Network::slotAcceptConnection() {
        shadapp::Logger::debug("NOUVEAU PEER CONNECTE");
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(slotReceiveCCMfromNewPeer()));
    }

    void Network::slotSocketConnected(shadapp::fs::Device *connectedDevice) {
        connectedPeers++;
        std::vector<shadapp::fs::Folder*> messageFolders;
        std::map<std::string, std::string> options;
        for (auto &folder : lp->getConfig()->getFolders()) {
            for (auto &device : folder->getDevices()) {
                if (device->getId().compare(connectedDevice->getId()) == 0) {
                    messageFolders.push_back(folder);
                }
            }
        }
        shadapp::protocol::ClusterConfigMessage ccm(
                *lp->getConfig()->getVersion(),
                lp->getConfig()->getID(),
                "V",
                messageFolders,
                options);
        send(connectedDevice->getSocket(), ccm);

    }

    void Network::slotSocketDisconnected() {
        connectedPeers--;
    }

    void Network::slotReceive(shadapp::fs::Device *device) {
        std::vector<uint8_t>* data = receive(device->getSocket());
        shadapp::Logger::debug("SWITCH MESSAGE");
        switch (shadapp::protocol::AbstractMessage::getType(*data)) {
            case shadapp::protocol::Type::CLUSTER_CONFIG:
            {
                shadapp::Logger::debug("CASE : CCM");
                shadapp::protocol::ClusterConfigMessage msg(*data);
                msg.executeAction(*device, *lp);
                shadapp::Logger::debug("CASE : CCM SORTIE");
            }
                break;
            case shadapp::protocol::Type::INDEX:
            {
                shadapp::Logger::debug("CASE : INDEX");
                shadapp::protocol::IndexMessage msg(*data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::REQUEST:
            {
                shadapp::Logger::debug("CASE : REQUEST");
                shadapp::protocol::RequestMessage msg(*data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::RESPONSE:
            {
                shadapp::Logger::debug("CASE : RESPONSE");
                shadapp::protocol::ResponseMessage msg(*data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::PING:
            {
                shadapp::Logger::debug("CASE : PING");
                shadapp::protocol::PingMessage msg(*data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::PONG:
            {
                shadapp::Logger::debug("CASE : PONG");
                shadapp::protocol::PongMessage msg(*data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::INDEX_UPDATE:
            {
                shadapp::protocol::IndexUpdateMessage msg(*data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::CLOSE:
            {
                shadapp::protocol::CloseMessage msg(*data);
                msg.executeAction(*device, *lp);
            }
                break;
        }
        delete data;
        //std::cout << "SOCKET :: byte available : " << device->getSocket()->bytesAvailable() << std::endl;
        if(device->getSocket()->bytesAvailable() > 0){
            slotReceive(device);
        }
        shadapp::Logger::debug("FIN SWITCH");        
    }

    void Network::slotReceiveCCMfromNewPeer() {
        QTcpSocket* socket = dynamic_cast<QTcpSocket*> (sender());
        disconnect(socket, 0, 0, 0);
        shadapp::Logger::debug("MESSAGE CCMSLOT");
        std::vector<uint8_t>* data = receive(socket);
        shadapp::protocol::ClusterConfigMessage ccm(*data);
        shadapp::fs::Device* device;
        //link socket to device
        for (auto &device_temp : lp->getConfig()->getDevices()) {
            if (device_temp->getId().compare(ccm.getClientName()) == 0) {
                device = device_temp;
            }
        }
        device->setSocket(socket);
        initQtSignals(device);

        //create CCM
        std::vector<shadapp::fs::Folder*> messageFolders;
        std::map<std::string, std::string> options;
        for (auto &folder : lp->getConfig()->getFolders()) {
            for (auto &device_temp : folder->getDevices()) {
                if (device_temp->getId().compare(ccm.getClientName()) == 0) {
                    messageFolders.push_back(folder);
                }
            }
        }
        shadapp::protocol::ClusterConfigMessage message(
                *(lp->getConfig()->getVersion()),
                lp->getConfig()->getName(),
                "V",
                messageFolders,
                options);
        send(device->getSocket(), message);
        ccm.executeAction(*device, *lp);
    }

}
