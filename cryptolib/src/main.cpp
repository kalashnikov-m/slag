/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include "gtest/gtest.h"

#include <algorithm.hpp>

#include <HugeTest.cpp>
#include <HugeCoreTest.cpp>

#include <vector>

using namespace std;

#define swap32(x) x

#define F1(x, y, z) ((x) & (y) ^ (~(x) & (z)))
#define F2(x, y, z) ((x) ^ (y) ^ (z))
#define F3(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define round1(a, b, c, d, e, k, w)                             \
    {                                                           \
        uint32_t temp = rotl32(a, 5) + F1(b, c, d) + e + k + w; \
        e             = d;                                      \
        d             = c;                                      \
        c             = (rotl32(b, 30));                        \
        b             = a;                                      \
        a             = temp;                                   \
    }

#define round2(a, b, c, d, e, k, w)                             \
    do                                                          \
    {                                                           \
        uint32_t temp = rotl32(a, 5) + F2(b, c, d) + e + k + w; \
        e             = d;                                      \
        d             = c;                                      \
        c             = (rotl32(b, 30));                        \
        b             = a;                                      \
        a = temp;                                               \
    } while (0);

#define round3(a, b, c, d, e, k, w)                             \
    do                                                          \
    {                                                           \
        uint32_t temp = rotl32(a, 5) + F3(b, c, d) + e + k + w; \
        e             = d;                                      \
        d             = c;                                      \
        c             = (rotl32(b, 30));                        \
        b             = a;                                      \
        a = temp;                                               \
    } while (0);

#define round4(a, b, c, d, e, k, w) round1(a, b, c, d, e, k, w)

class Sha1
{

  public:
    Sha1() : m_Idx(0), m_Len(0)
    {

        m_Digest[0] = 0x67452301;
        m_Digest[1] = 0xefcdab89;
        m_Digest[2] = 0x98badcfe;
        m_Digest[3] = 0x10325476;
        m_Digest[4] = 0xc3d2e1f0;
    }

    ~Sha1()
    {
    }

    void Update(const uint8_t in[], int inlen)
    {
        while (inlen--)
        {
            // printf("%x\n", *in);
            m_Block[m_Idx++] = *in++;
            m_Len += 8;
            // printf("%x\n", m_Block[m_Idx - 1]);

            if (m_Idx == 0x40) // 64
            {
                sha1Transform();

                m_Idx = 0;
            }
        }
    }

    void Final(/*std::vector<uint8_t>& digest*/)
    {
        if (m_Idx > 55)
        {
            m_Block[m_Idx++] = 0x80;
            while (m_Idx < 64)
            {
                m_Block[m_Idx++] = 0x00;
            }

            sha1Transform();

            while (m_Idx < 56)
            {
                m_Block[m_Idx++] = 0x00;
            }
        }
        else
        {
            m_Block[m_Idx++] = 0x80;

            while (m_Idx < 56)
            {
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

        sha1Transform();

         //printf("0x%08x\n", m_Digest[0]);
         //printf("0x%08x\n", m_Digest[1]);
         //printf("0x%08x\n", m_Digest[2]);
         //printf("0x%08x\n", m_Digest[3]);
         //printf("0x%08x\n", m_Digest[4]);
    }

  protected:
    uint32_t rotl32(uint32_t x, int shift)
    {
        while (shift-- > 0)
        {
            x = ((x << 1) | (x >> 31));
        }

        return x;
    }

    void sha1Transform()
    {
        uint32_t words[80] = {0x00};

        uint32_t a, b, c, d, e;

        // digest init
        a = m_Digest[0];
        b = m_Digest[1];
        c = m_Digest[2];
        d = m_Digest[3];
        e = m_Digest[4];

        for (int t = 0; t < 16; ++t)
        {
            words[t] = m_Block[t * 4] << 24;      // swap32(m_Words[i]);
            words[t] |= m_Block[t * 4 + 1] << 16; // swap32(m_Words[i]);
            words[t] |= m_Block[t * 4 + 2] << 8;  // swap32(m_Words[i]);
            words[t] |= m_Block[t * 4 + 3];       // swap32(m_Words[i]);
        }

        for (int j = 0; j < 16; ++j)
        {
            printf("W[%d] = %08x\n", j, words[j]);
        }

        for (int i = 16; i < 80; ++i)
        {
            words[i] = rotl32(words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16], 1);
        }

        // main cycle
        printf("-----------------------------------------\n");
        for (int i = 0; i < 20; ++i)
        {
            round1(a, b, c, d, e, 0x5A827999, words[i]);
            printf("t = %d: <a = %08x, b = %08x, c = %08x, d = %08x, e = %08x>\n", i, a, b, c, d, e);
        }

	printf("-----------------------------------------\n");
        for (int i = 20; i < 40; ++i)
        {
            round2(a, b, c, d, e, 0x6ED9EBA1, words[i]);
            printf("t = %d: <a = %08x, b = %08x, c = %08x, d = %08x, e = %08x>\n", i, a, b, c, d, e);
        }

	printf("-----------------------------------------\n");
        for (int i = 40; i < 60; ++i)
        {
            round3(a, b, c, d, e, 0x8F1BBCDC, words[i]);
            printf("t = %d: <a = %08x, b = %08x, c = %08x, d = %08x, e = %08x>\n", i, a, b, c, d, e);
        }

	printf("-----------------------------------------\n");
        for (int i = 60; i < 80; ++i)
        {
            round4(a, b, c, d, e, 0xCA62C1D6, words[i]);
            printf("t = %d: <a = %08x, b = %08x, c = %08x, d = %08x, e = %08x>\n", i, a, b, c, d, e);
        }

        m_Digest[0] += a;
        m_Digest[1] += b;
        m_Digest[2] += c;
        m_Digest[3] += d;
        m_Digest[4] += e;
    }

  private:
    uint32_t m_Digest[5]; // 160

    uint8_t m_Block[64]; // 512
    uint8_t m_Idx;

    union
    {
        uint64_t m_Len;
        uint32_t high;
        uint32_t low;
    };
};

int main(int argc, char** argv)
{
    // cout<<rotl32(5, 1)<<"\n";

    const uint8_t d[] = {'a', 'b', 'c'};

    Sha1 hash;
    hash.Update(d, sizeof(d) / sizeof(uint8_t));
    hash.Final();

    return 0;
    //::testing::InitGoogleTest(&argc, argv);

    // return RUN_ALL_TESTS();
}
