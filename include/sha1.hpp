#ifndef SHA1_HPP
#define SHA1_HPP

#include <cstdint>

namespace cry {

    class SHA1 {

      public:
        SHA1() {}
        ~SHA1() {}

        static const size_t size = 20;

        void Init() {
            m_Idx = 0;
            m_Len = 0;

            m_Digest[0] = 0x67452301;
            m_Digest[1] = 0xefcdab89;
            m_Digest[2] = 0x98badcfe;
            m_Digest[3] = 0x10325476;
            m_Digest[4] = 0xc3d2e1f0;
        }

        template <class InputIterator, class OutputIterator>
        void operator()(InputIterator first, InputIterator last, OutputIterator result) {

            Init();
            Update(first, last);
            Final(result);
        }

        template <class InputIterator>
        void Update(InputIterator first, InputIterator last) {

            while (first != last) {
                m_Block[m_Idx++] = *first++;
                m_Len += 8;

                if (m_Idx == 0x40) // 64
                {
                    transform();
                }
            }
        }

        template <class OutputIterator>
        void Final(OutputIterator result) {
            if (m_Idx > 55) {
                m_Block[m_Idx++] = 0x80;
                while (m_Idx < 64) {
                    m_Block[m_Idx++] = 0x00;
                }

                transform();

                while (m_Idx < 56) {
                    m_Block[m_Idx++] = 0x00;
                }
            } else {
                m_Block[m_Idx++] = 0x80;

                while (m_Idx < 56) {
                    m_Block[m_Idx++] = 0x00;
                }
            }

            m_Block[m_Idx++] = (m_Len & 0xFFFFFFFF00000000) >> 24;
            m_Block[m_Idx++] = (m_Len & 0xFFFFFFFF00000000) >> 16;
            m_Block[m_Idx++] = (m_Len & 0xFFFFFFFF00000000) >> 8;
            m_Block[m_Idx++] = (m_Len & 0xFFFFFFFF00000000);

            m_Block[m_Idx++] = (m_Len & 0x00000000FFFFFFFF) >> 24;
            m_Block[m_Idx++] = (m_Len & 0x00000000FFFFFFFF) >> 16;
            m_Block[m_Idx++] = (m_Len & 0x00000000FFFFFFFF) >> 8;
            m_Block[m_Idx++] = (m_Len & 0x00000000FFFFFFFF);

            transform();

            *result++ = (m_Digest[0] >> 24) & 0x000000ff;
            *result++ = (m_Digest[0] >> 16) & 0x000000ff;
            *result++ = (m_Digest[0] >> 8) & 0x000000ff;
            *result++ = (m_Digest[0] >> 0) & 0x000000ff;

            *result++ = (m_Digest[1] >> 24) & 0x000000ff;
            *result++ = (m_Digest[1] >> 16) & 0x000000ff;
            *result++ = (m_Digest[1] >> 8) & 0x000000ff;
            *result++ = (m_Digest[1] >> 0) & 0x000000ff;

            *result++ = (m_Digest[2] >> 24) & 0x000000ff;
            *result++ = (m_Digest[2] >> 16) & 0x000000ff;
            *result++ = (m_Digest[2] >> 8) & 0x000000ff;
            *result++ = (m_Digest[2] >> 0) & 0x000000ff;

            *result++ = (m_Digest[3] >> 24) & 0x000000ff;
            *result++ = (m_Digest[3] >> 16) & 0x000000ff;
            *result++ = (m_Digest[3] >> 8) & 0x000000ff;
            *result++ = (m_Digest[3] >> 0) & 0x000000ff;

            *result++ = (m_Digest[4] >> 24) & 0x000000ff;
            *result++ = (m_Digest[4] >> 16) & 0x000000ff;
            *result++ = (m_Digest[4] >> 8) & 0x000000ff;
            *result++ = (m_Digest[4] >> 0) & 0x000000ff;
        }

      protected:
        uint32_t inline ROTL(uint32_t x, int shift) { return ((x << shift) | (x >> (32 - shift))); }

        uint32_t inline Ch(uint32_t x, uint32_t y, uint32_t z) { return ((x & y) ^ (~(x) & (z))); }

        uint32_t inline Parity(uint32_t x, uint32_t y, uint32_t z) { return ((x) ^ (y) ^ (z)); }

        uint32_t inline Maj(uint32_t x, uint32_t y, uint32_t z) { return ((x & y) ^ (x & z) ^ (y & z)); }

        void transform() {
            uint32_t W[80] = {0x00};

            uint32_t a, b, c, d, e;

            // digest init
            a = m_Digest[0];
            b = m_Digest[1];
            c = m_Digest[2];
            d = m_Digest[3];
            e = m_Digest[4];

            for (int t = 0; t < 16; ++t) {
                W[t] = m_Block[t * 4] << 24;
                W[t] |= m_Block[t * 4 + 1] << 16;
                W[t] |= m_Block[t * 4 + 2] << 8;
                W[t] |= m_Block[t * 4 + 3];
            }

            for (int i = 16; i < 80; ++i) {
                W[i] = ROTL(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
            }

            // main cycle
            uint32_t T = 0;

            for (int t = 0; t < 80; ++t) {
                if (t < 20) {
                    T = ROTL(a, 5) + Ch(b, c, d) + e + 0x5A827999 + W[t];
                } else if (t < 40) {
                    T = ROTL(a, 5) + Parity(b, c, d) + e + 0x6ED9EBA1 + W[t];
                } else if (t < 60) {
                    T = ROTL(a, 5) + Maj(b, c, d) + e + 0x8F1BBCDC + W[t];
                } else if (t < 80) {
                    T = ROTL(a, 5) + Parity(b, c, d) + e + 0xCA62C1D6 + W[t];
                }

                e = d;
                d = c;
                c = ROTL(b, 30);
                b = a;
                a = T;
            }

            m_Digest[0] += a;
            m_Digest[1] += b;
            m_Digest[2] += c;
            m_Digest[3] += d;
            m_Digest[4] += e;

            m_Idx = 0;
        }

      private:
        uint32_t m_Digest[5]; // 160
        uint8_t m_Block[64];  // 512
        uint8_t m_Idx;
        uint64_t m_Len;
    };
}

#endif
