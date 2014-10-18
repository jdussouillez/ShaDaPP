#ifndef DEVICE_H
#define	DEVICE_H

#include <cstdint>
#include <string>

#include <shadapp/data/Serializable.h>

namespace shadapp {

    namespace fs {

        class Device : public shadapp::data::Serializable {
        private:
            std::string id;
            uint32_t flags;
            uint64_t maxLocalVersion;

        public:
            explicit Device(std::string id, uint32_t flags, uint64_t maxLocalVersion);

            std::string getId() const;
            uint32_t getFlags() const;
            uint64_t getMaxLocalVersion() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;

            static Device getFromBytes(unsigned char* bytes, unsigned int* size);
        };
    }
}

#endif	// DEVICE_H
