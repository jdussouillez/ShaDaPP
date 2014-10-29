#ifndef ABSTRACTMESSAGE_H
#define	ABSTRACTMESSAGE_H

#include <bitset>
#include <string>

#include <shadapp/data/Serializable.h>

namespace shadapp {

    namespace protocol {

        enum Type : uint8_t { // 1 byte enum
            CLUSTER_CONFIG = 0,
            INDEX,
            REQUEST,
            RESPONSE,
            PING,
            PONG,
            INDEX_UPDATE,
            CLOSE
        };

        class Message : public shadapp::data::Serializable {
        private:
            std::bitset<4> version;
            std::bitset<12> id; // 1,5 byte
            Type type;
            bool compressed;

        protected:
            explicit Message(std::bitset<4> version, Type type, bool compressed);
            explicit Message(std::bitset<12> id, std::bitset<4> version, Type type, bool compressed);

        public:
            explicit Message(std::vector<uint8_t>* bytes, bool erase = true);

            std::bitset<4> getVersion() const;
            std::bitset<12> getId() const;
            Type getType() const;
            bool isCompressed() const;

            std::vector<uint8_t>* serialize(std::vector<uint8_t>* bytes) const override;
        };
    }
}

#endif	// ABSTRACTMESSAGE_H