#ifndef SHA512_HPP
#define SHA512_HPP

#include <cstdint>

namespace cry {

    class SHA512 {

      public:
        SHA512() {}
        ~SHA512() {}

        static const size_t size = 64;

        void Init() {
            m_Idx = 0;
            high = low = 0;

            m_Digest[0] = 0x6a09e667f3bcc908;
            m_Digest[1] = 0xbb67ae8584caa73b;
            m_Digest[2] = 0x3c6ef372fe94f82b;
            m_Digest[3] = 0xa54ff53a5f1d36f1;
            m_Digest[4] = 0x510e527fade682d1;
            m_Digest[5] = 0x9b05688c2b3e6c1f;
            m_Digest[6] = 0x1f83d9abfb41bd6b;
            m_Digest[7] = 0x5be0cd19137e2179;
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
                low += 8;
                if (low == 0x00) {
                    high++;
                    if (high == 0x00) {
                        throw 1;
                    }
                }

                if (m_Idx == 0x80) // 128
                {
                    transform();
                }
            }
        }

        template <class OutputIterator>
        void Final(OutputIterator result) {
            if (m_Idx > 111) {
                m_Block[m_Idx++] = 0x80;
                while (m_Idx < 128) {
                    m_Block[m_Idx++] = 0x00;
                }

                transform();

                while (m_Idx < 112) {
                    m_Block[m_Idx++] = 0x00;
                }
            } else {
                m_Block[m_Idx++] = 0x80;

                while (m_Idx < 112) {
                    m_Block[m_Idx++] = 0x00;
                }
            }

            m_Block[m_Idx++] = high >> 56;
            m_Block[m_Idx++] = high >> 48;
            m_Block[m_Idx++] = high >> 40;
            m_Block[m_Idx++] = high >> 32;
            m_Block[m_Idx++] = high >> 24;
            m_Block[m_Idx++] = high >> 16;
            m_Block[m_Idx++] = high >> 8;
            m_Block[m_Idx++] = high >> 0;

            m_Block[m_Idx++] = low >> 56;
            m_Block[m_Idx++] = low >> 48;
            m_Block[m_Idx++] = low >> 40;
            m_Block[m_Idx++] = low >> 32;
            m_Block[m_Idx++] = low >> 24;
            m_Block[m_Idx++] = low >> 16;
            m_Block[m_Idx++] = low >> 8;
            m_Block[m_Idx++] = low >> 0;

            transform();

            *result++ = (m_Digest[0] >> 56) & 0x00000000000000ff;
            *result++ = (m_Digest[0] >> 48) & 0x00000000000000ff;
            *result++ = (m_Digest[0] >> 40) & 0x00000000000000ff;
            *result++ = (m_Digest[0] >> 32) & 0x00000000000000ff;
            *result++ = (m_Digest[0] >> 24) & 0x00000000000000ff;
            *result++ = (m_Digest[0] >> 16) & 0x00000000000000ff;
            *result++ = (m_Digest[0] >> 8) & 0x00000000000000ff;
            *result++ = (m_Digest[0] >> 0) & 0x00000000000000ff;

            *result++ = (m_Digest[1] >> 56) & 0x00000000000000ff;
            *result++ = (m_Digest[1] >> 48) & 0x00000000000000ff;
            *result++ = (m_Digest[1] >> 40) & 0x00000000000000ff;
            *result++ = (m_Digest[1] >> 32) & 0x00000000000000ff;
            *result++ = (m_Digest[1] >> 24) & 0x00000000000000ff;
            *result++ = (m_Digest[1] >> 16) & 0x00000000000000ff;
            *result++ = (m_Digest[1] >> 8) & 0x00000000000000ff;
            *result++ = (m_Digest[1] >> 0) & 0x00000000000000ff;

            *result++ = (m_Digest[2] >> 56) & 0x00000000000000ff;
            *result++ = (m_Digest[2] >> 48) & 0x00000000000000ff;
            *result++ = (m_Digest[2] >> 40) & 0x00000000000000ff;
            *result++ = (m_Digest[2] >> 32) & 0x00000000000000ff;
            *result++ = (m_Digest[2] >> 24) & 0x00000000000000ff;
            *result++ = (m_Digest[2] >> 16) & 0x00000000000000ff;
            *result++ = (m_Digest[2] >> 8) & 0x00000000000000ff;
            *result++ = (m_Digest[2] >> 0) & 0x00000000000000ff;

            *result++ = (m_Digest[3] >> 56) & 0x00000000000000ff;
            *result++ = (m_Digest[3] >> 48) & 0x00000000000000ff;
            *result++ = (m_Digest[3] >> 40) & 0x00000000000000ff;
            *result++ = (m_Digest[3] >> 32) & 0x00000000000000ff;
            *result++ = (m_Digest[3] >> 24) & 0x00000000000000ff;
            *result++ = (m_Digest[3] >> 16) & 0x00000000000000ff;
            *result++ = (m_Digest[3] >> 8) & 0x00000000000000ff;
            *result++ = (m_Digest[3] >> 0) & 0x00000000000000ff;

            *result++ = (m_Digest[4] >> 56) & 0x00000000000000ff;
            *result++ = (m_Digest[4] >> 48) & 0x00000000000000ff;
            *result++ = (m_Digest[4] >> 40) & 0x00000000000000ff;
            *result++ = (m_Digest[4] >> 32) & 0x00000000000000ff;
            *result++ = (m_Digest[4] >> 24) & 0x00000000000000ff;
            *result++ = (m_Digest[4] >> 16) & 0x00000000000000ff;
            *result++ = (m_Digest[4] >> 8) & 0x00000000000000ff;
            *result++ = (m_Digest[4] >> 0) & 0x00000000000000ff;

            *result++ = (m_Digest[5] >> 56) & 0x00000000000000ff;
            *result++ = (m_Digest[5] >> 48) & 0x00000000000000ff;
            *result++ = (m_Digest[5] >> 40) & 0x00000000000000ff;
            *result++ = (m_Digest[5] >> 32) & 0x00000000000000ff;
            *result++ = (m_Digest[5] >> 24) & 0x00000000000000ff;
            *result++ = (m_Digest[5] >> 16) & 0x00000000000000ff;
            *result++ = (m_Digest[5] >> 8) & 0x00000000000000ff;
            *result++ = (m_Digest[5] >> 0) & 0x00000000000000ff;

            *result++ = (m_Digest[6] >> 56) & 0x00000000000000ff;
            *result++ = (m_Digest[6] >> 48) & 0x00000000000000ff;
            *result++ = (m_Digest[6] >> 40) & 0x00000000000000ff;
            *result++ = (m_Digest[6] >> 32) & 0x00000000000000ff;
            *result++ = (m_Digest[6] >> 24) & 0x00000000000000ff;
            *result++ = (m_Digest[6] >> 16) & 0x00000000000000ff;
            *result++ = (m_Digest[6] >> 8) & 0x00000000000000ff;
            *result++ = (m_Digest[6] >> 0) & 0x00000000000000ff;

            *result++ = (m_Digest[7] >> 56) & 0x00000000000000ff;
            *result++ = (m_Digest[7] >> 48) & 0x00000000000000ff;
            *result++ = (m_Digest[7] >> 40) & 0x00000000000000ff;
            *result++ = (m_Digest[7] >> 32) & 0x00000000000000ff;
            *result++ = (m_Digest[7] >> 24) & 0x00000000000000ff;
            *result++ = (m_Digest[7] >> 16) & 0x00000000000000ff;
            *result++ = (m_Digest[7] >> 8) & 0x00000000000000ff;
            *result++ = (m_Digest[7] >> 0) & 0x00000000000000ff;
        }

      protected:
        uint64_t inline ROTR(uint64_t x, int shift) { return ((x >> shift) | (x << (64 - shift))); }

        uint64_t inline SUM0(uint64_t x) { return (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39)); }

        uint64_t inline SUM1(uint64_t x) { return (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41)); }

        uint64_t inline sigma_0(uint64_t x) { return (ROTR(x, 1) ^ ROTR(x, 8) ^ (x >> 7)); }

        uint64_t inline sigma_1(uint64_t x) { return (ROTR(x, 19) ^ ROTR(x, 61) ^ (x >> 6)); }

        uint64_t inline Ch(uint64_t x, uint64_t y, uint64_t z) { return ((x & y) ^ (~(x) & (z))); }

        uint64_t inline Maj(uint64_t x, uint64_t y, uint64_t z) { return ((x & y) ^ (x & z) ^ (y & z)); }

        void transform() {
            uint64_t W[80] = {0x00};

            uint64_t a, b, c, d, e, f, g, h;

            // 1. Prepare the message schedule
            for (int t = 0; t < 16; ++t) {

                W[t] |= uint64_t(m_Block[t * 8 + 0]) << 56;
                W[t] |= uint64_t(m_Block[t * 8 + 1]) << 48;
                W[t] |= uint64_t(m_Block[t * 8 + 2]) << 40;
                W[t] |= uint64_t(m_Block[t * 8 + 3]) << 32;
                W[t] |= uint64_t(m_Block[t * 8 + 4]) << 24;
                W[t] |= uint64_t(m_Block[t * 8 + 5]) << 16;
                W[t] |= uint64_t(m_Block[t * 8 + 6]) << 8;
                W[t] |= uint64_t(m_Block[t * 8 + 7]) << 0;
            }

            for (int t = 16; t < 80; ++t) {
                W[t] = sigma_1(W[t - 2]) + W[t - 7] + sigma_0(W[t - 15]) + W[t - 16];
            }

            // 2. Initialize  the  eight  working  variables,
            a = m_Digest[0];
            b = m_Digest[1];
            c = m_Digest[2];
            d = m_Digest[3];
            e = m_Digest[4];
            f = m_Digest[5];
            g = m_Digest[6];
            h = m_Digest[7];

            // main cycle
            uint64_t T1 = 0;
            uint64_t T2 = 0;

            for (int t = 0; t < 80; ++t) {

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
        uint64_t m_Digest[8]; // 512
        uint8_t m_Block[128]; // 1024
        uint8_t m_Idx;

        struct {
            uint64_t high;
            uint64_t low;
        };

        const uint64_t K[80] = {
            0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
            0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
            0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
            0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
            0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
            0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
            0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817,
        };
    };
}

#endif
