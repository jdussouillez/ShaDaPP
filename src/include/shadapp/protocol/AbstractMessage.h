#ifndef ABSTRACTMESSAGE_H
#define	ABSTRACTMESSAGE_H

#include <bitset>
#include <string>

#include <shadapp/protocol/Serializable.h>

namespace shadapp {

    namespace protocol {

        enum Type : unsigned char { // 1 byte enum
            CLUSTER_CONFIG = 0,
            INDEX,
            REQUEST,
            RESPONSE,
            PING,
            PONG,
            INDEX_UPDATE,
            CLOSE
        };

        class AbstractMessage : public Serializable {
        private:
            std::bitset<4> version;
            std::bitset<12> id; // 1,5 byte
            Type type;
            bool compressed;

        protected:
            explicit AbstractMessage(std::bitset<4> version, Type type, bool compressed);
            explicit AbstractMessage(std::bitset<12> id, std::bitset<4> version, Type type, bool compressed);
            explicit AbstractMessage(unsigned char* bytes);

        public:
            std::bitset<4> getVersion() const;
            std::bitset<12> getId() const;
            Type getType() const;
            bool isCompressed() const;

            unsigned char* serialize(unsigned char* dest, unsigned int* size) const override;
        };
    }
}

#endif	// ABSTRACTMESSAGE_H
