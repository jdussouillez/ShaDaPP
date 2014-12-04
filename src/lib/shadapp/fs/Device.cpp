#include <shadapp/data/Serializer.h>
#include <shadapp/fs/Device.h>

#include <iostream>
#include <qt4/QtNetwork/qtcpsocket.h>

namespace shadapp {

    namespace fs {

        /*
         * 
         * Constructors & destructors
         * 
         */
        Device::Device(std::string id, std::string name,
                std::string address, unsigned short port,
                uint32_t flags, uint64_t maxLocalVersion)
        : id(id), name(name), address(address), port(port),
        flags(flags), maxLocalVersion(maxLocalVersion) {
            socket = new QTcpSocket(0);
           
        }

        Device::Device(std::string id, uint32_t flags, uint64_t maxLocalVersion)
        : Device(id, "", "", 0, flags, maxLocalVersion) {
        }

        Device::Device(std::string id)
        : Device(id, 0, 0) {
        }

        Device::Device(std::vector<uint8_t>& bytes) {
            uint32_t idLength = shadapp::data::Serializer::deserializeInt32(bytes);
            id = shadapp::data::Serializer::deserializeString(bytes, idLength);
            flags = shadapp::data::Serializer::deserializeInt32(bytes);
            maxLocalVersion = shadapp::data::Serializer::deserializeInt64(bytes);
        }

        Device::~Device() {
            //TODO: memory fix
            delete socket;
        }

        /*
         * 
         * Getters
         * 
         */
        std::string Device::getId() const {
            return id;
        }

        std::string Device::getName() const {
            return name;
        }

        std::string Device::getAddress() const {
            return address;
        }

        unsigned short Device::getPort() const {
            return port;
        }

        uint64_t Device::getMaxLocalVersion() const {
            return maxLocalVersion;
        }

        QTcpSocket* Device::getSocket() {
            return socket;
        }

        bool Device::isTrusted() const {
            return flags & shadapp::fs::TRUSTED;
        }

        bool Device::isReadOnly() const {
            return flags & shadapp::fs::READ_ONLY;
        }

        bool Device::isIntroducer() const {
            return flags & shadapp::fs::INTRODUCER;
        }

        /*
         * 
         * Setters
         * 
         */
        void Device::setName(std::string name) {
            this->name = name;
        }

        void Device::setAddress(std::string address) {
            this->address = address;
        }

        void Device::setPort(unsigned short port) {
            this->port = port;
        }        
        
        void Device::setSocket(QTcpSocket* sock){
            socket = sock;
        }

        void Device::setTrusted(bool trust) {
            flags = trust ?
                    flags | shadapp::fs::DeviceFlag::TRUSTED
                    : flags & ~shadapp::fs::DeviceFlag::TRUSTED;
        }

        void Device::setReadOnly(bool readOnly) {
            flags = readOnly ?
                    flags | shadapp::fs::DeviceFlag::READ_ONLY
                    : flags & ~shadapp::fs::DeviceFlag::READ_ONLY;
        }

        void Device::setIntroducer(bool introducer) {
            flags = introducer ?
                    flags | shadapp::fs::DeviceFlag::INTRODUCER
                    : flags & ~shadapp::fs::DeviceFlag::INTRODUCER;
        }

        /*
         * 
         * Others
         * 
         */
        std::vector<uint8_t> Device::serialize() const {
            std::vector<uint8_t> bytes;
            shadapp::data::Serializer::serializeInt32(bytes, id.length());
            shadapp::data::Serializer::serializeString(bytes, id);
            shadapp::data::Serializer::serializeInt32(bytes, flags);
            shadapp::data::Serializer::serializeInt64(bytes, maxLocalVersion);
            return bytes;
        }
        
        void Device::slotDeviceConnected(){
            std::cout<<"conencted from device"<<std::endl;
            emit signalConnected(this);
        }
        
        void Device::slotReceive(){
            emit signalReceive(this);
        }

    }
}
