#include <shadapp/data/Serializer.h>
#include <shadapp/fs/Device.h>

namespace shadapp {

    namespace fs {

        /*
         * 
         * Constructors & destructors
         * 
         */
        Device::Device(std::string id, std::string name,
                std::string address, unsigned short port,
                uint32_t flags, uint32_t maxLocalVersion)
        : id(id), name(name), address(address), port(port),
        flags(flags), maxLocalVersion(maxLocalVersion) {
        }

        Device::Device(std::string id, uint32_t flags, uint32_t maxLocalVersion)
        : Device(id, "", "", 0, flags, maxLocalVersion) {
        }

        Device::Device(std::string id)
        : Device(id, 0, 0) {
        }

        Device::~Device() {
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
        unsigned char* Device::serialize(unsigned char* dest, unsigned int* size) const {
            shadapp::data::Serializer::serializeInt32(dest, *size, id.length(), size);
            shadapp::data::Serializer::serializeString(dest, *size, id, size);
            shadapp::data::Serializer::serializeInt32(dest, *size, flags, size);
            shadapp::data::Serializer::serializeInt64(dest, *size, maxLocalVersion, size);
            return dest;
        }

        Device* Device::getFromBytes(unsigned char* bytes, unsigned int* size) {
            uint32_t idLength = shadapp::data::Serializer::deserializeInt32(bytes, size);
            std::string id = shadapp::data::Serializer::deserializeString(bytes, size, idLength);
            uint32_t flags = shadapp::data::Serializer::deserializeInt32(bytes, size);
            uint64_t maxLocalVersion = shadapp::data::Serializer::deserializeInt64(bytes, size);
            return new Device(id, flags, maxLocalVersion);
        }
    }
}
