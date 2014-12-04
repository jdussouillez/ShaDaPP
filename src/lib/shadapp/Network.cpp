#include <bitset>
#include <iostream>

#include <chrono>
#include <thread>
#include <stdio.h>

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
#define connectType Qt::AutoConnection


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

    int Network::send(QTcpSocket *peer, const shadapp::protocol::Message& msg) {
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

    std::vector<uint8_t>* Network::receive(QTcpSocket* socket) {
        int sizeRead = socket->bytesAvailable();
        std::cout << "size read : " << sizeRead << std::endl;
        //socket->peek
        std::vector<uint8_t> *data = new std::vector<uint8_t>(sizeRead);
        socket->read((char*) &data->at(0), sizeRead);
        //socket->flush();
        return data;
    }

    void Network::slotAcceptConnection() {
        std::cout << "nouveau peer" << std::endl;
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(slotReceiveCCMfromNewPeer()));
    }

    void Network::slotSocketConnected(shadapp::fs::Device *connectedDevice) {
        connectedPeers++;
        std::vector<shadapp::fs::Folder> messageFolders;
        std::map<std::string, std::string> options;
        for (auto &folder : lp->getConfig()->getFolders()) {
            for (auto &device : folder.getDevices()) {
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
        std::cout << "CCM envoye a " << connectedDevice->getId() << "sizeOf : " << sizeof (ccm) << std::endl;
        send(connectedDevice->getSocket(), ccm);

    }

    void Network::slotSocketDisconnected() {
        connectedPeers--;
    }

    void Network::slotReceive(shadapp::fs::Device *device) {
        std::vector<uint8_t>* data = receive(device->getSocket());
        std::cout << "MESSAGE" << std::endl;
        switch (shadapp::protocol::Message::getType(*data)) {
            case shadapp::protocol::Type::CLUSTER_CONFIG:
            {
                std::cout << "CASE : CCM" << std::endl;
                shadapp::protocol::ClusterConfigMessage msg(data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::INDEX:
            {
                std::cout << "CASE : INDEX size : " << std::endl;
//                shadapp::protocol::IndexMessage msg(data);
//                msg.executeAction(*device, *lp);
//                std::cout << "CASE : INDEX END" << std::endl;
            }
                break;
            case shadapp::protocol::Type::REQUEST:
            {
                shadapp::protocol::RequestMessage msg(data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::RESPONSE:
            {
                shadapp::protocol::ResponseMessage msg(data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::PING:
            {
                shadapp::protocol::PingMessage msg(data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::PONG:
            {
                shadapp::protocol::PongMessage msg(data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::INDEX_UPDATE:
            {
                shadapp::protocol::IndexUpdateMessage msg(data);
                msg.executeAction(*device, *lp);
            }
                break;
            case shadapp::protocol::Type::CLOSE:
            {
                shadapp::protocol::CloseMessage msg(data);
                msg.executeAction(*device, *lp);
            }
                break;
        }
        delete data;
    }

    void Network::slotReceiveCCMfromNewPeer() {
        QTcpSocket* socket = dynamic_cast<QTcpSocket*> (sender());
        disconnect(socket, 0, 0, 0);
        //disconnect(socket, SIGNAL(readyRead()), this, SLOT(slotReceiveCCMfromNewPeer()));
        std::cout << "enter in ccm slot" << std::endl;
        std::vector<uint8_t>* data = receive(socket);
        shadapp::protocol::ClusterConfigMessage ccm(data);
        shadapp::fs::Device* device;
        //link socket to device
        for (auto &device_temp : lp->getConfig()->getDevices()) {
            if (device_temp->getId().compare(ccm.getClientName()) == 0) {
                device = device_temp;
            }
        }
        device->setSocket(socket);
        ////EN TEST
        initQtSignals(device);
//        connect(device->getSocket(), SIGNAL(readyRead()),
//                device, SLOT(slotReceive()), connectType);
//        //connect(device->getSocket(), SIGNAL(readyRead()),
//          //      device, SLOT(slotReceive()), connectType);
//        connect(device, SIGNAL(signalReceive(shadapp::fs::Device*)),
//                this, SLOT(slotReceive(shadapp::fs::Device*)), connectType);
        ////////////////////////

        std::cout << "Receive CCM from " << device->getId() << std::endl;


        //create CCM
        std::vector<shadapp::fs::Folder> messageFolders;
        std::map<std::string, std::string> options;
        for (auto &folder : lp->getConfig()->getFolders()) {
            for (auto &device_temp : folder.getDevices()) {
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
        std::cout << "CCM envoye a " << device->getId() << "sizeOf : " << sizeof (message) << std::endl;
        send(device->getSocket(), message);

        //update config
        ccm.executeAction(*device, *lp);
        //TODO: enlever
        //lp->sendAllIndexMessage(device, message.getFolders());
    }

}
