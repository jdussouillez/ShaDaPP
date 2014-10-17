#ifndef SERIALIZABLE_H
#define	SERIALIZABLE_H

namespace shadapp {

    namespace protocol {

        class Serializable {
        public:
            virtual unsigned char* serialize(unsigned char* dest, unsigned int* size) const = 0;
        };
    }
}

#endif	// SERIALIZABLE_H
