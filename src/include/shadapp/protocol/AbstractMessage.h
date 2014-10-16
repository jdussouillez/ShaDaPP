#ifndef ABSTRACTMESSAGE_H
#define	ABSTRACTMESSAGE_H

#include <bitset>
#include <string>

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

        class AbstractMessage {
        private:
            std::bitset<4> version;
            std::bitset<12> id; // 1,5 byte
            Type type;
            bool compressed;

        protected:
            explicit AbstractMessage(std::bitset<4> version, Type type, bool compressed);
            explicit AbstractMessage(std::bitset<12> id, std::bitset<4> version, Type type, bool compressed);

        public:
            std::bitset<4> getVersion() const;
            std::bitset<12> getId() const;
            Type getType() const;
            bool isCompressed() const;
            void serialize(char* dest, int* size) const;
        };
    }
}

#endif	// ABSTRACTMESSAGE_H
