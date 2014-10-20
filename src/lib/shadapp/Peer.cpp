#include <QtCore/QIODevice>
#include <QtCore/QString>

#include <shadapp/Core.h>
#include <shadapp/Peer.h>
#include <shadapp/protocol/AbstractMessage.h>

namespace shadapp {

    bool Peer::connexionToPeer(std::string name,
            std::string id,
            unsigned int port) {

        QTcpSocket socket;
        socket.connectToHost(QString(name.c_str()), port, QIODevice::ReadWrite);
        if (socket.state() != QAbstractSocket::ConnectedState) {
            return false;
        }
        sockets[id] = &socket;
        return true;
    }

    int Peer::send(std::string id, shadapp::protocol::AbstractMessage msg) {
        QTcpSocket *socket = sockets[id];
        unsigned char data[MAX_MESSAGE_SIZE];
        unsigned int size;
        msg.serialize(data, &size);
        unsigned int sizeSend = socket->write((const char*) data, size);
        if (sizeSend != size) {
            return 0;
        }
        return sizeSend;
    }

    int Peer::receive(std::string id, char* data) {
        QTcpSocket *socket = sockets[id];
        int sizeRead = socket->readBufferSize();
        socket->read(data, sizeRead);
        return sizeRead;
    }
}

