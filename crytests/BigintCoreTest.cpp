
#include "gtest/gtest.h"

#include <bigint_core.hpp>

#include <cctype>

using namespace std;

template <class InputIterator>
static bool ASSERT_BYTES_EQ(InputIterator f1, InputIterator l1, InputIterator f2, InputIterator l2)
{

    for (; (f1 != l1) && (*f1 == 0x00); ++f1)
        ;

    for (; (f2 != l2) && (*f2 == 0x00); ++f2)
        ;

    for (; (f1 != l1) && (f2 != l2) && (*f1) == (*f2); ++f1, ++f2)
        ;

    return (f1 == l1) && (f2 == l2);
}

class BigintCoreTest : public ::testing::Test
{

  protected:
    template <class T>
    void addition(const std::string& a, const std::string& b, const std::string& expected)
    {
        auto aa = hex2polynomial<T>(a);
        auto bb = hex2polynomial<T>(b);
        auto ex = hex2polynomial<T>(expected);

        std::vector<T> actual(10);

        Cry_add<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

        bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
        EXPECT_TRUE(eq);
    }

    template <class P>
    std::vector<P> hex2polynomial(const std::string& hex)
    {
        if (hex.empty())
        {
            return std::vector<P>();
        }

        /////////////////////////////////
        // skiping zeros and whitespaces
        auto it(hex.begin());
        for (; it != hex.end() && ((*it == '0') || (*it == ' '));)
            ++it;

        std::string::const_reverse_iterator rit(hex.rbegin()), rend(it);

        //////////////////////////////////
        // counting hexadecimal characters
        size_t nchars = std::count_if(rit, rend, [](unsigned char c) { return std::isalnum(c); });
        size_t nbytes = nchars / 2;
        nbytes += nchars % 2;

        size_t nwords = nbytes / sizeof(P);
        if (nbytes % sizeof(P))
            nwords += 1;

        P word     = 0;
        size_t cnt = 0;

        std::vector<P> dst(nwords);
        auto ret(dst.rbegin());

        for (; rit != rend; ++rit)
        {
            uint8_t bt = *rit;

            //////////////////////////////////////////////////////////
            // transform hex character to the 4bit equivalent number,
            // using the ascii table indexes
            if (bt >= '0' && bt <= '9')
                bt = bt - '0';
            else if (bt >= 'a' && bt <= 'f')
                bt = bt - 'a' + 10;
            else if (bt >= 'A' && bt <= 'F')
                bt = bt - 'A' + 10;
            else if (bt == ' ')
                continue;
            else
                throw std::logic_error("Invalid hex string. Only hex numbers are allowed.");

            ////////////////////////////////////////////////////////////////////////////
            // shift 4 to make space for new digit, and add the 4 bits of the new digit
            // word = (word << 4) | (bt & 0xF);
            word = (static_cast<P>(bt) << static_cast<P>(cnt * 4)) | (word);

            ++cnt;

            if (cnt == sizeof(P) * 2)
            {
                *ret++ = word;
                word   = 0;
                cnt    = 0;
            }
        }

        if (ret != dst.rend())
            *ret++ = word;

        return dst;
    }
};

TEST_F(BigintCoreTest, Cry_add)
{
    addition<uint8_t>("00ff", "00", "ff");
    addition<uint8_t>("001a03", "000011", "1a14");
    addition<uint8_t>("00ffff", "0000ffff", "01FFFE");
    addition<uint8_t>("000001fa14bace680235", "00000a1405f5ef382a14", "0c0e1ab0bda02c49");
    addition<uint8_t>("10", "ff", "010f");
    addition<uint8_t>("ffffffffff", "01", "010000000000");

    addition<uint16_t>("00ff", "00", "ff");
    addition<uint16_t>("001a03", "000011", "1a14");
    addition<uint16_t>("00ffff", "0000ffff", "01FFFE");
    addition<uint16_t>("000001fa14bace680235", "00000a1405f5ef382a14", "0c0e1ab0bda02c49");
    addition<uint16_t>("10", "ff", "010f");
    addition<uint16_t>("ffffffffff", "01", "010000000000");

    addition<uint32_t>("00ff", "00", "ff");
    addition<uint32_t>("001a03", "000011", "1a14");
    addition<uint32_t>("00ffff", "0000ffff", "01FFFE");
    addition<uint32_t>("000001fa14bace680235", "00000a1405f5ef382a14", "0c0e1ab0bda02c49");
    addition<uint32_t>("10", "ff", "010f");
    addition<uint32_t>("ffffffffff", "01", "010000000000");
}
