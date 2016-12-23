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
    Sha1() : m_Inlen(0)
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
            m_In[m_Inlen++] = *in++;

            if (m_Inlen == 0x40)
            {
                memcpy(m_Words, m_In, m_Inlen);

                sha1Transform();

                m_Inlen = 0;
            }
        }
    }

    /*void Final(std::vector<uint8_t>& digest)
    {
    }*/

  protected:
    uint32_t rotl32(uint32_t x, int shift)
    {
        while (shift-- > 0)
        {
            x = ((x << 1) | (x & 0x8000000) >> 31);
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

        for (int i = 0; i < 16; ++i)
        {
            words[i] = swap32(m_Words[i]);
        }

        for (int i = 16; i < 80; ++i)
        {
            words[i] = rotl32(m_Words[i - 3] ^ m_Words[i - 8] ^ m_Words[i - 14] ^ m_Words[i - 16], 1);
        }

        // main cycle
        for (int i = 0; i < 20; ++i)
        {
            round1(a, b, c, d, e, 0x5A827999, words[i]);
        }

        for (int i = 20; i < 40; ++i)
        {
             round2(a, b, c, d, e, 0x6ED9EBA1, words[i]);
        }

        for (int i = 40; i < 60; ++i)
        {
             round3(a, b, c, d, e, 0x8F1BBCDC, words[i]);
        }

        for (int i = 60; i < 80; ++i)
        {
             round4(a, b, c, d, e, 0xCA62C1D6, words[i]);
        }

        m_Digest[0] += a;
        m_Digest[1] += b;
        m_Digest[2] += c;
        m_Digest[3] += d;
        m_Digest[4] += e;
    }

  private:
    uint32_t m_Digest[5]; // 160
    uint32_t m_Words[16]; // 512
    uint8_t m_In[64];
    uint64_t m_Inlen;
};

int main(int argc, char** argv)
{
    cout<<rotl32(5, 1)<<"\n";
    
    //const uint8_t d[] = {};

    //Sha1 hash;

    //hash.Update(d, sizeof(d));
    // hash.Final();

    return 0;
    //::testing::InitGoogleTest(&argc, argv);

    // return RUN_ALL_TESTS();
}
