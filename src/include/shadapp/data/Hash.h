#ifndef HASH_H
#define HASH_H

#include <cstdint>
#include <string>

namespace shadapp {

    namespace data {

        class Hash256 {
        public:
            static bool hash(const uint8_t* input, uint32_t size, uint8_t output[32]);

        private:
            uint32_t m_h[8];
            uint64_t m_bitLength;
            uint32_t m_dataLength;
            uint8_t m_data[64];
            
            Hash256();
            virtual ~Hash256();
            
            void init();
            void update(const uint8_t* input, uint32_t size);
            void hash();
            void getHash(uint8_t bytes[32]);
            
            inline uint32_t rotr(uint32_t x, uint32_t y);
            inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
            inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z);
            inline uint32_t ep0(uint32_t x);
            inline uint32_t ep1(uint32_t x);
            inline uint32_t sig0(uint32_t x);
            inline uint32_t sig1(uint32_t x);
        };
    }
}

#endif	// HASH_H
