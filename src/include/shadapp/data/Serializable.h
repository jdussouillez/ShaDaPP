#ifndef SERIALIZABLE_H
#define	SERIALIZABLE_H

#include <cstdlib>
#include <vector>

namespace shadapp {

    namespace data {

        class Serializable {
        public:
            virtual std::vector<uint8_t> serialize() const = 0;
        };
    }
}

#endif	// SERIALIZABLE_H
