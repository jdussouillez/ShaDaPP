#ifndef DEVICE_H
#define	DEVICE_H

#include <cstdint>
#include <string>

#include <shadapp/data/Serializable.h>

namespace shadapp {

    namespace fs {

        enum DeviceFlag : uint32_t {
            TRUSTED = 0x0001,
            READ_ONLY = 0x0002,
            INTRODUCER = 0x0004
        };

        class Device : public shadapp::data::Serializable {
        private:
            std::string id;
            std::string name;
            std::string address;
            unsigned short port;
            uint32_t flags;
            uint64_t maxLocalVersion;

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
            bool isTrusted() const;
            bool isReadOnly() const;
            bool isIntroducer() const;

            void setName(std::string name);
            void setAddress(std::string address);
            void setPort(unsigned short port);
            void setTrusted(bool trust);
            void setReadOnly(bool readOnly);
            void setIntroducer(bool introducer);

            std::vector<uint8_t> serialize() const override;
        };
    }
}

#endif	// DEVICE_H
