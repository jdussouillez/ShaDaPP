#include <shadapp/data/Serializer.h>
#include <shadapp/fs/Device.h>

namespace shadapp {

    namespace fs {

        Device::Device(std::string id, uint32_t flags, uint64_t maxLocalVersion)
        : id(id), flags(flags), maxLocalVersion(maxLocalVersion) {
        }

        std::string Device::getId() const {
            return id;
        }

        uint32_t Device::getFlags() const {
            return flags;
        }

        uint64_t Device::getMaxLocalVersion() const {
            return maxLocalVersion;
        }

        unsigned char* Device::serialize(unsigned char* dest, unsigned int* size) const {
            shadapp::data::Serializer::serializeInt32(dest, *size, id.length(), size);
            shadapp::data::Serializer::serializeString(dest, *size, id, size);
            shadapp::data::Serializer::serializeInt32(dest, *size, flags, size);
            shadapp::data::Serializer::serializeInt64(dest, *size, maxLocalVersion, size);
            return dest;
        }

        Device Device::getFromBytes(unsigned char* bytes, unsigned int* size) {
            uint32_t idLength = shadapp::data::Serializer::deserializeInt32(bytes, size);
            std::string id = shadapp::data::Serializer::deserializeString(bytes, size, idLength);
            uint32_t flags = shadapp::data::Serializer::deserializeInt32(bytes, size);
            uint64_t maxLocalVersion = shadapp::data::Serializer::deserializeInt64(bytes, size);
            return Device(id, flags, maxLocalVersion);
        }
    }
}
