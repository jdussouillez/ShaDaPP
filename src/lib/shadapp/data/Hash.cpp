#include <cstring>
#include <iostream>
#include <sstream>

#include <shadapp/data/Hash.h>

namespace shadapp {

    namespace data {

        bool Hash256::hash(const uint8_t* input, uint32_t size, uint8_t output[32]) {
            if (input == nullptr) {
                return false;
            }
            Hash256 h;
            h.init();
            h.update(input, size);
            h.getHash(output);
            return true;
        }
        
        bool Hash256::hash(const uint8_t* input, uint32_t size, std::string& hexStr) {
            uint8_t hash[32];
            if (!Hash256::hash(input, size, hash)) {
                return false;
            }
            std::ostringstream oss;
            oss << "0x" << std::hex;
            for (int i = 0; i < 32; i++) {
                oss << static_cast<uint16_t>(hash[i]);
            }
            hexStr = oss.str();
            return true;
        }

        static const uint32_t k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

        Hash256::Hash256() {
        }

        Hash256::~Hash256() {
        }

        void Hash256::init() {
            m_h[0] = 0x6a09e667;
            m_h[1] = 0xbb67ae85;
            m_h[2] = 0x3c6ef372;
            m_h[3] = 0xa54ff53a;
            m_h[4] = 0x510e527f;
            m_h[5] = 0x9b05688c;
            m_h[6] = 0x1f83d9ab;
            m_h[7] = 0x5be0cd19;
            m_bitLength = 0;
            m_dataLength = 0;
        }

        void Hash256::update(const uint8_t* input, uint32_t size) {
            for (uint32_t i = 0; i < size; i++) {
                m_data[m_dataLength] = input[i];
                m_dataLength++;
                if (m_dataLength == 64) {
                    hash();
                    m_bitLength += 512;
                    m_dataLength = 0;
                }
            }
        }

        void Hash256::hash() {
            uint32_t a, b, c, d, e, f, g, h, i, j, tmp1, tmp2, m[64];
            for (i = 0, j = 0; i < 16; ++i, j += 4) {
                m[i] = (m_data[j] << 24) | (m_data[j + 1] << 16) | (m_data[j + 2] << 8) | (m_data[j + 3]);
            }
            for (; i < 64; ++i) {
                m[i] = sig1(m[i - 2]) + m[i - 7] + sig0(m[i - 15]) + m[i - 16];
            }
            a = m_h[0];
            b = m_h[1];
            c = m_h[2];
            d = m_h[3];
            e = m_h[4];
            f = m_h[5];
            g = m_h[6];
            h = m_h[7];
            for (i = 0; i < 64; ++i) {
                tmp1 = h + ep1(e) + ch(e, f, g) + k[i] + m[i];
                tmp2 = ep0(a) + maj(a, b, c);
                h = g;
                g = f;
                f = e;
                e = d + tmp1;
                d = c;
                c = b;
                b = a;
                a = tmp1 + tmp2;
            }
            m_h[0] += a;
            m_h[1] += b;
            m_h[2] += c;
            m_h[3] += d;
            m_h[4] += e;
            m_h[5] += f;
            m_h[6] += g;
            m_h[7] += h;
        }

        void Hash256::getHash(uint8_t bytes[32]) {
            uint32_t i = m_dataLength;

            if (m_dataLength < 56) {
                m_data[i++] = 0x80;
                while (i < 56) {
                    m_data[i++] = 0x00;
                }
            } else {
                m_data[i++] = 0x80;
                while (i < 64) {
                    m_data[i++] = 0x00;
                }
                hash();
                std::memset(m_data, 0, 56);
            }

            m_bitLength += m_dataLength * 8;
            m_data[63] = m_bitLength;
            m_data[62] = m_bitLength >> 8;
            m_data[61] = m_bitLength >> 16;
            m_data[60] = m_bitLength >> 24;
            m_data[59] = m_bitLength >> 32;
            m_data[58] = m_bitLength >> 40;
            m_data[57] = m_bitLength >> 48;
            m_data[56] = m_bitLength >> 56;
            hash();

            for (i = 0; i < 8; i++) {
                bytes[4 * i] = (m_h[i] >> 24) & 0x000000ff;
                bytes[4 * i + 1] = (m_h[i] >> 16) & 0x000000ff;
                bytes[4 * i + 2] = (m_h[i] >> 8) & 0x000000ff;
                bytes[4 * i + 3] = m_h[i] >> 0 & 0x000000ff;
            }
        }

        uint32_t Hash256::rotr(uint32_t x, uint32_t y) {
            return ((x) >> (y)) | ((x) << (32 - (y)));
        }

        uint32_t Hash256::ch(uint32_t x, uint32_t y, uint32_t z) {
            return ((x) & (y)) ^ (~(x) & (z));
        }

        uint32_t Hash256::maj(uint32_t x, uint32_t y, uint32_t z) {
            return ((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z));
        }

        uint32_t Hash256::ep0(uint32_t x) {
            return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
        }

        uint32_t Hash256::ep1(uint32_t x) {
            return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
        }

        uint32_t Hash256::sig0(uint32_t x) {
            return rotr(x, 7) ^ rotr(x, 18) ^ ((x) >> 3);
        }

        uint32_t Hash256::sig1(uint32_t x) {
            return rotr(x, 17) ^ rotr(x, 19) ^ ((x) >> 10);
        }
    }
}
