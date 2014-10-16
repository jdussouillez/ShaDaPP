#include <shadapp/fs/Device.h>

namespace shadapp {

    namespace fs {

        Device::Device(std::string id, uint32_t flag, uint64_t maxLocalVersion)
        : id(id), flag(flag), maxLocalVersion(maxLocalVersion) {
        }

        std::string Device::getId() const {
            return id;
        }

        uint32_t Device::getFlag() const {
            return flag;
        }

        uint64_t Device::getMaxLocalVersion() const {
            return maxLocalVersion;
        }
    }
}
