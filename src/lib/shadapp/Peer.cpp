#include <QtCore/QIODevice>
#include <QtCore/QString>

#include <shadapp/Core.h>
#include <shadapp/Peer.h>

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
        QTcpSocket* socket = sockets[id];
        std::vector<uint8_t> bytes = msg.serialize();
        unsigned int sizeSend = socket->write((const char*) &bytes.at(0), bytes.size());
        if (sizeSend != bytes.size()) {
            return 0;
        }
        return sizeSend;
    }

    int Peer::receive(std::string id, char* data) {
        QTcpSocket* socket = sockets[id];
        int sizeRead = socket->readBufferSize();
        socket->read(data, sizeRead);
        return sizeRead;
    }
}
