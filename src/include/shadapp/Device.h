#ifndef DEVICE_H
#define	DEVICE_H

#include <stdint.h>
#include <string>

namespace shadapp {

    class Device {
    private:
        std::string id;
        uint32_t flag;
        uint64_t maxLocalVersion;

    public:
        explicit Device(std::string id, uint32_t flag, uint64_t maxLocalVersion);

        std::string getId() const;
        uint32_t getFlag() const;
        uint64_t getMaxLocalVersion() const;
    };
}

#endif	// DEVICE_H
