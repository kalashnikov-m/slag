#ifndef SHA224_HPP
#define SHA224_HPP

#include <cstdint>

namespace cry {

    class SHA224 {

      public:
        SHA224(): m_Idx(0), m_Len(0)
	    {
	    }

	    ~SHA224() {}

        static const size_t size = 28;

        void Init() {
            m_Idx = 0;
            m_Len = 0;

            m_Digest[0] = 0xc1059ed8;
            m_Digest[1] = 0x367cd507;
            m_Digest[2] = 0x3070dd17;
            m_Digest[3] = 0xf70e5939;
            m_Digest[4] = 0xffc00b31;
            m_Digest[5] = 0x68581511;
            m_Digest[6] = 0x64f98fa7;
            m_Digest[7] = 0xbefa4fa4;
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

            m_Block[m_Idx++] = static_cast<uint8_t>((m_Len & 0xFFFFFFFF00000000) >> 24);
            m_Block[m_Idx++] = static_cast<uint8_t>((m_Len & 0xFFFFFFFF00000000) >> 16);
            m_Block[m_Idx++] = static_cast<uint8_t>((m_Len & 0xFFFFFFFF00000000) >> 8);
            m_Block[m_Idx++] = static_cast<uint8_t>((m_Len & 0xFFFFFFFF00000000));

            m_Block[m_Idx++] = static_cast<uint8_t>((m_Len & 0x00000000FFFFFFFF) >> 24);
            m_Block[m_Idx++] = static_cast<uint8_t>((m_Len & 0x00000000FFFFFFFF) >> 16);
            m_Block[m_Idx++] = static_cast<uint8_t>((m_Len & 0x00000000FFFFFFFF) >> 8);
            m_Block[m_Idx++] = static_cast<uint8_t>((m_Len & 0x00000000FFFFFFFF));

            transform();

            // result.resize(28);

            result[0] = (m_Digest[0] >> 24) & 0x000000ff;
            result[1] = (m_Digest[0] >> 16) & 0x000000ff;
            result[2] = (m_Digest[0] >> 8) & 0x000000ff;
            result[3] = (m_Digest[0] >> 0) & 0x000000ff;

            result[4] = (m_Digest[1] >> 24) & 0x000000ff;
            result[5] = (m_Digest[1] >> 16) & 0x000000ff;
            result[6] = (m_Digest[1] >> 8) & 0x000000ff;
            result[7] = (m_Digest[1] >> 0) & 0x000000ff;

            result[8] = (m_Digest[2] >> 24) & 0x000000ff;
            result[9] = (m_Digest[2] >> 16) & 0x000000ff;
            result[10] = (m_Digest[2] >> 8) & 0x000000ff;
            result[11] = (m_Digest[2] >> 0) & 0x000000ff;

            result[12] = (m_Digest[3] >> 24) & 0x000000ff;
            result[13] = (m_Digest[3] >> 16) & 0x000000ff;
            result[14] = (m_Digest[3] >> 8) & 0x000000ff;
            result[15] = (m_Digest[3] >> 0) & 0x000000ff;

            result[16] = (m_Digest[4] >> 24) & 0x000000ff;
            result[17] = (m_Digest[4] >> 16) & 0x000000ff;
            result[18] = (m_Digest[4] >> 8) & 0x000000ff;
            result[19] = (m_Digest[4] >> 0) & 0x000000ff;

            result[20] = (m_Digest[5] >> 24) & 0x000000ff;
            result[21] = (m_Digest[5] >> 16) & 0x000000ff;
            result[22] = (m_Digest[5] >> 8) & 0x000000ff;
            result[23] = (m_Digest[5] >> 0) & 0x000000ff;

            result[24] = (m_Digest[6] >> 24) & 0x000000ff;
            result[25] = (m_Digest[6] >> 16) & 0x000000ff;
            result[26] = (m_Digest[6] >> 8) & 0x000000ff;
            result[27] = (m_Digest[6] >> 0) & 0x000000ff;
        }

      protected:
	    static uint32_t inline ROTR(uint32_t x, int shift) { return ((x >> shift) | (x << (32 - shift))); }

	    static uint32_t inline SUM0(uint32_t x) { return (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22)); }

	    static uint32_t inline SUM1(uint32_t x) { return (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25)); }

	    static uint32_t inline sigma_0(uint32_t x) { return (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3)); }

	    static uint32_t inline sigma_1(uint32_t x) { return (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10)); }

	    static uint32_t inline Ch(uint32_t x, uint32_t y, uint32_t z) { return ((x & y) ^ (~(x) & (z))); }

	    static uint32_t inline Maj(uint32_t x, uint32_t y, uint32_t z) { return ((x & y) ^ (x & z) ^ (y & z)); }

        void transform() {
            uint32_t W[64] = {0x00};

	        // digest init
            uint32_t a = m_Digest[0];
            uint32_t b = m_Digest[1];
            uint32_t c = m_Digest[2];
            uint32_t d = m_Digest[3];
            uint32_t e = m_Digest[4];
            uint32_t f = m_Digest[5];
            uint32_t g = m_Digest[6];
            uint32_t h = m_Digest[7];

            for (int t = 0; t < 16; ++t) {
                W[t] = m_Block[t * 4] << 24;
                W[t] |= m_Block[t * 4 + 1] << 16;
                W[t] |= m_Block[t * 4 + 2] << 8;
                W[t] |= m_Block[t * 4 + 3];
            }

            for (int t = 16; t < 64; ++t) {
                W[t] = sigma_1(W[t - 2]) + W[t - 7] + sigma_0(W[t - 15]) + W[t - 16];
            }

            // main cycle
            uint32_t T1 = 0;
            uint32_t T2 = 0;

            for (int t = 0; t < 64; ++t) {

                T1 = h + SUM1(e) + Ch(e, f, g) + K[t] + W[t];
                T2 = SUM0(a) + Maj(a, b, c);

                h = g;
                g = f;
                f = e;
                e = d + T1;
                d = c;
                c = b;
                b = a;
                a = T1 + T2;
            }

            m_Digest[0] += a;
            m_Digest[1] += b;
            m_Digest[2] += c;
            m_Digest[3] += d;
            m_Digest[4] += e;
            m_Digest[5] += f;
            m_Digest[6] += g;
            m_Digest[7] += h;

            m_Idx = 0;
        }

      private:
        uint32_t m_Digest[8]; // 256
        uint8_t m_Block[64];  // 512
        uint8_t m_Idx;
        uint64_t m_Len;

        const uint32_t K[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa,
            0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
            0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
        };
    };
}

#endif
