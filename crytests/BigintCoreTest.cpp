
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
    void addition(const std::string& a, const std::string& b, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(ex.size());

            Cry_add<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(ex.size());

            Cry_add<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(ex.size());

            Cry_add<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }
    }

    void subtract(const std::string& a, const std::string& b, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);

            Cry_subtract<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);

            Cry_subtract<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);

            Cry_subtract<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }
    }

    void multiply(const std::string& a, const std::string& b, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);

            Cry_multiply<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);

            Cry_multiply<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);

            Cry_multiply<T>(begin(aa), end(aa), begin(bb), end(bb), end(actual));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }
    }

	void increment(const std::string& a, const std::string& expected)
    {
		{
			using T = uint8_t;
			auto aa = hex2polynomial<T>(a);
			auto ex = hex2polynomial<T>(expected);

			std::vector<T> actual(15);

			Cry_Increment<T>(begin(aa), end(aa));

			bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
			EXPECT_TRUE(eq);
		}

		{
			using T = uint16_t;
			auto aa = hex2polynomial<T>(a);
			auto ex = hex2polynomial<T>(expected);

			std::vector<T> actual(15);

			Cry_Increment<T>(begin(aa), end(aa));

			bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
			EXPECT_TRUE(eq);
		}

		{
			using T = uint32_t;
			auto aa = hex2polynomial<T>(a);
			auto ex = hex2polynomial<T>(expected);

			std::vector<T> actual(15);

			Cry_Increment<T>(begin(aa), end(aa));

			bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
			EXPECT_TRUE(eq);
		}
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
    addition("00ff", "00", "ff");
    addition("001a03", "000011", "1a14");
    addition("00ffff", "0000ffff", "01FFFE");
    addition("000001fa14bace680235", "00000a1405f5ef382a14", "0c0e1ab0bda02c49");
    addition("10", "ff", "010f");
    addition("ffffffffff", "01", "010000000000");
}

TEST_F(BigintCoreTest, Cry_subtract)
{
    subtract("0101", "02", "ff");
    subtract("000100", "0000000000000100", "00");
    subtract("00000000000000000101", "00000080", "0081");
    subtract("ffffffffffffffffffffffff", "01", "fffffffffffffffffffffffe");
    subtract("ffffffff", "ffffffff", "0000");
    subtract("00ff", "00", "ff");
    subtract("001a03", "000011", "19F2");
    subtract("00ffff", "0000ffff", "00");
    subtract("f4ab41fa14bace68", "00000a1405f5ef382a14", "EA973C042582A454");
    subtract("ffffffffff", "01", "fffffffffe");
}

TEST_F(BigintCoreTest, Cry_multiply)
{
    multiply("00000000000000001a03", "00000011", "01BA33");
    multiply("00ffff", "0000ffff", "FFFE0001");
    multiply("00ff", "00", "00");
    multiply("02", "0080", "000100");
    multiply("1232AF42", "1232AF42", "14B2AAE35C34D04");
}

TEST_F(BigintCoreTest, Cry_Increment)
{
	increment("000001", "02");
	increment("0000ff", "000100");
	increment("00ffff", "010000");
	increment("00fffe", "00ffff");
}