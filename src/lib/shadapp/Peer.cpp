#include <QtCore/QIODevice>
#include <QtCore/QString>

#include <shadapp/Core.h>
#include <shadapp/Peer.h>

#include <shadapp/protocol/PingMessage.h>

namespace shadapp {

    Peer::Peer(shadapp::fs::Device* device) {
        peerDevice = device;
    }

   
//    bool Peer::connexionToPeer(std::string name,
//            std::string id,
//            unsigned int port) {
//        QTcpSocket socket;
//        socket.connectToHost(QString(name.c_str()), port, QIODevice::ReadWrite);
//        if (socket.state() != QAbstractSocket::ConnectedState) {
//            return false;
//        }
//        sockets[id] = &socket;
//        return true;
//    }
//
//    int Peer::send(std::string id, shadapp::protocol::AbstractMessage msg) {
//        QTcpSocket* socket = sockets[id];
//        std::vector<uint8_t> bytes;
//        if (msg.serialize(&bytes) == nullptr) {
//            // TODO: error
//        }
//        unsigned int sizeSend = socket->write((const char*) &bytes.at(0), bytes.size());
//        if (sizeSend != bytes.size()) {
//            return 0;
//        }
//        return sizeSend;
//    }

//    int Peer::ping(std::bitset<4>* version) {
//        
//    }

    //    bool Peer::connexionToPeer(std::string name,
    //            std::string id,
    //            unsigned int port) {
    //
    //        QTcpSocket socket;
    //        socket.connectToHost(QString(name.c_str()), port, QIODevice::ReadWrite);
    //        if (socket.state() != QAbstractSocket::ConnectedState) {
    //            return false;
    //        }
    //        sockets[id] = &socket;
    //        return true;
    //    }
    //
    //    int Peer::send(std::string id, shadapp::protocol::AbstractMessage msg) {
    //        QTcpSocket* socket = sockets[id];
    //        unsigned char data[MAX_MESSAGE_SIZE];
    //        unsigned int size;
    //        msg.serialize(data, &size);
    //        unsigned int sizeSend = socket->write((const char*) data, size);
    //        if (sizeSend != size) {
    //            return 0;
    //        }
    //        return sizeSend;
    //    }
    //
    //    int Peer::receive(std::string id, char* data) {
    //        QTcpSocket* socket = sockets[id];
    //        int sizeRead = socket->readBufferSize();
    //        socket->read(data, sizeRead);
    //        return sizeRead;
    //    }
}
