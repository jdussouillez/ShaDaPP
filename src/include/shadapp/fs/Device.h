#ifndef DEVICE_H
#define	DEVICE_H

#include <cstdint>
#include <string>

#include <QtNetwork/QTcpSocket>

#include <shadapp/data/Serializable.h>

namespace shadapp {

    namespace fs {

        enum DeviceFlag : uint32_t {
            TRUSTED = 0x0001,
            READ_ONLY = 0x0002,
            INTRODUCER = 0x0004
        };

        class Device : public QObject, public shadapp::data::Serializable {
            Q_OBJECT
        private:
            std::string id;
            std::string name;
            std::string address;
            unsigned short port;
            uint32_t flags;
            uint64_t maxLocalVersion;
            QTcpSocket *socket;

        public:
            explicit Device(std::string id, std::string name,
                    std::string address, unsigned short port,
                    uint32_t flags, uint64_t maxLocalVersion);
            explicit Device(std::string id, uint32_t flags, uint64_t maxLocalVersion);
            explicit Device(std::string id);
            explicit Device(std::vector<uint8_t>& bytes);
            virtual ~Device();

            std::string getId() const;
            std::string getName() const;
            std::string getAddress() const;
            unsigned short getPort() const;
            uint64_t getMaxLocalVersion() const;
            QTcpSocket* getSocket();
            bool isTrusted() const;
            bool isReadOnly() const;
            bool isIntroducer() const;

            void setName(std::string name);
            void setAddress(std::string address);
            void setPort(unsigned short port);
            void setSocket(QTcpSocket* sock);
            void setTrusted(bool trust);
            void setReadOnly(bool readOnly);
            void setIntroducer(bool introducer);

            std::vector<uint8_t> serialize() const override;

        signals:
            void signalConnected(shadapp::fs::Device *device);
            void signalReceive(shadapp::fs::Device *device);
        public slots:
            void slotDeviceConnected();
            void slotReceive();
        };
    }
}

#endif	// DEVICE_H
