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

class SHA1
{

  public:
    SHA1() : m_Idx(0), m_Len(0)
    {

        m_Digest[0] = 0x67452301;
        m_Digest[1] = 0xefcdab89;
        m_Digest[2] = 0x98badcfe;
        m_Digest[3] = 0x10325476;
        m_Digest[4] = 0xc3d2e1f0;
    }

    ~SHA1()
    {
    }

    void Update(const uint8_t in[], uint64_t inlen)
    {
        printf("%ld", inlen);
        
        while (inlen--)
        {
            m_Block[m_Idx++] = *in++;
            m_Len += 8;
         
            if (m_Idx == 0x40) // 64
            {
                transform();
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

            transform();

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

        transform();

        //for (int i = 0; i < 20; ++i)
            //printf("%02x", m_Hash[i]);
         printf("0x%08x\n", m_Digest[0]);
         printf("0x%08x\n", m_Digest[1]);
         printf("0x%08x\n", m_Digest[2]);
         printf("0x%08x\n", m_Digest[3]);
         printf("0x%08x\n", m_Digest[4]);
    }

  protected:
    uint32_t inline ROTL(uint32_t x, int shift)
    {
        x = ((x << shift) | (x >> (32 - shift)));

        return x;
    }
    
    uint32_t inline Ch(uint32_t x, uint32_t y, uint32_t z) { return ((x) & (y) ^ (~(x) & (z))); }
    
    uint32_t inline Parity(uint32_t x, uint32_t y, uint32_t z) { return ((x) ^ (y) ^ (z)); }
    
    uint32_t inline Maj(uint32_t x, uint32_t y, uint32_t z) { return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z))); } 

    void transform()
    {
        uint32_t W[80] = {0x00};

        uint32_t a, b, c, d, e;

        for (int i = 0; i < 64; ++i)
        {
            printf("%02x:", m_Block[i]);
        }
        
        // digest init
        a = m_Digest[0];
        b = m_Digest[1];
        c = m_Digest[2];
        d = m_Digest[3];
        e = m_Digest[4];

        for (int t = 0; t < 16; ++t)
        {
            W[t] = m_Block[t * 4] << 24;      
            W[t] |= m_Block[t * 4 + 1] << 16; 
            W[t] |= m_Block[t * 4 + 2] << 8;  
            W[t] |= m_Block[t * 4 + 3];
            
            printf("0x%08x\n", W[t]);
        }

        for (int j = 0; j < 16; ++j)
        {
            printf("W[%d] = %08x\n", j, W[j]);
        }

        for (int i = 16; i < 80; ++i)
        {
            W[i] = ROTL(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
        }

        // main cycle
        printf("-----------------------------------------\n");
        //{
            uint32_t T = 0;
            
            
            for (int t = 0; t < 80; ++t) {
            
            if (t < 20)
            {
                T = ROTL(a, 5) + Ch(b, c, d) + e + 0x5A827999 + W[t];
            }
            else
            if (t < 40)
            {
                T = ROTL(a, 5) + Parity(b, c, d) + e + 0x6ED9EBA1 + W[t];
            }
            else
            if (t < 60)
            {
                T = ROTL(a, 5) + Maj(b, c, d) + e + 0x8F1BBCDC + W[t];
            }
            else
            if (t < 80)
            {
                T = ROTL(a, 5) + Parity(b, c, d) + e + 0xCA62C1D6 + W[t];
            }
            
            e = d;                                      
            d = c;                                      
            c = ROTL(b, 30);
            b = a;                                      
            a = T;
            
            printf("t = %d: <a = %08x, b = %08x, c = %08x, d = %08x, e = %08x>\n", t, a, b, c, d, e);
            
            }        
        
        m_Digest[0] += a;
        m_Digest[1] += b;
        m_Digest[2] += c;
        m_Digest[3] += d;
        m_Digest[4] += e;
        
        m_Idx = 0;
        //std::for_each(std::begin(m_Hash), std::end(m_Hash), [](uint8_t bt) { std::cout<<bt; });
        
        /*cout<<std::hex;
        for (int j = 0; j < 20; ++j)
            cout<<m_Hash[j];*/
    }

  private:
      union {
        uint8_t  m_Hash[20]; 
        uint32_t m_Digest[5]; // 160
      };

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

    //const uint8_t d[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
    const uint8_t d[1000000] = {'a'};

    //printf("%ld", sizeof(d) / sizeof(uint8_t));
    
    SHA1 hash;
    hash.Update(d, sizeof(d) / sizeof(uint8_t));
    hash.Final();

    return 0;
    //::testing::InitGoogleTest(&argc, argv);

    // return RUN_ALL_TESTS();
}
