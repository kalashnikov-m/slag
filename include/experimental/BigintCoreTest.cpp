
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

    void multiply_1(const std::string& a, const std::string& b, const std::string& expected)
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

	void multiply_2(const std::string& a, uint32_t b, const std::string& expected)
	{
		{
			using T = uint8_t;
			auto aa = hex2polynomial<T>(a);
			auto ex = hex2polynomial<T>(expected);

			std::vector<T> actual(15);

			Cry_multiply<T>(begin(aa), end(aa), b, end(actual));

			bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
			EXPECT_TRUE(eq);
		}

		{
			using T = uint16_t;
			auto aa = hex2polynomial<T>(a);
			auto ex = hex2polynomial<T>(expected);

			std::vector<T> actual(15);

			Cry_multiply<T>(begin(aa), end(aa), b, end(actual));

			bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
			EXPECT_TRUE(eq);
		}

		{
			using T = uint32_t;
			auto aa = hex2polynomial<T>(a);
			auto ex = hex2polynomial<T>(expected);

			std::vector<T> actual(15);

			Cry_multiply<T>(begin(aa), end(aa), b, end(actual));

			bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
			EXPECT_TRUE(eq);
		}
	}

    void div_rem(const std::string& a, const std::string& b, const std::string& div_expected, const std::string& rem_expected)
    {
        {
            using T     = uint8_t;
            auto aa     = hex2polynomial<T>(a);
            auto bb     = hex2polynomial<T>(b);
            auto div_ex = hex2polynomial<T>(div_expected);
            auto rem_ex = hex2polynomial<T>(rem_expected);

            std::vector<T> div(15);
            std::vector<T> rem(15);

            Cry_div_rem<T>(begin(aa), end(aa), begin(bb), end(bb), end(div), end(rem));

            bool eq1 = ASSERT_BYTES_EQ(std::begin(div_ex), std::end(div_ex), std::begin(div), std::end(div));
            EXPECT_TRUE(eq1);

            bool eq2 = ASSERT_BYTES_EQ(std::begin(rem_ex), std::end(rem_ex), std::begin(rem), std::end(rem));
            EXPECT_TRUE(eq2);
        }

        {
            using T     = uint16_t;
            auto aa     = hex2polynomial<T>(a);
            auto bb     = hex2polynomial<T>(b);
            auto div_ex = hex2polynomial<T>(div_expected);
            auto rem_ex = hex2polynomial<T>(rem_expected);

            std::vector<T> div(15);
            std::vector<T> rem(15);

            Cry_div_rem<T>(begin(aa), end(aa), begin(bb), end(bb), end(div), end(rem));

            bool eq1 = ASSERT_BYTES_EQ(std::begin(div_ex), std::end(div_ex), std::begin(div), std::end(div));
            EXPECT_TRUE(eq1);

            bool eq2 = ASSERT_BYTES_EQ(std::begin(rem_ex), std::end(rem_ex), std::begin(rem), std::end(rem));
            EXPECT_TRUE(eq2);
        }

        {
            using T     = uint32_t;
            auto aa     = hex2polynomial<T>(a);
            auto bb     = hex2polynomial<T>(b);
            auto div_ex = hex2polynomial<T>(div_expected);
            auto rem_ex = hex2polynomial<T>(rem_expected);

            std::vector<T> div(15);
            std::vector<T> rem(15);

            Cry_div_rem<T>(begin(aa), end(aa), begin(bb), end(bb), end(div), end(rem));

            bool eq1 = ASSERT_BYTES_EQ(std::begin(div_ex), std::end(div_ex), std::begin(div), std::end(div));
            EXPECT_TRUE(eq1);

            bool eq2 = ASSERT_BYTES_EQ(std::begin(rem_ex), std::end(rem_ex), std::begin(rem), std::end(rem));
            EXPECT_TRUE(eq2);
        }
    }

    void increment(const std::string& a, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_increment<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_increment<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_increment<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }
    }

    void decrement(const std::string& a, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_decrement<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_decrement<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_decrement<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }
    }

    void rotl(const std::string& a, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_rotl<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_rotl<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_rotl<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }
    }

    void rotr(const std::string& a, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_decrement<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_decrement<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_decrement<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }
    }

    void rshift(const std::string& a, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_rshift<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_rshift<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_rshift<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }
    }

    void lshift(const std::string& a, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_lshift<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_lshift<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            Cry_lshift<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }
    }

    void f_and(const std::string& a, const std::string& b, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_and<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_and<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_and<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }
    }

    void f_xor(const std::string& a, const std::string& b, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_xor<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_xor<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_xor<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }
    }

    void f_or(const std::string& a, const std::string& b, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_or<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_or<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto bb = hex2polynomial<T>(b);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_or<T>(begin(aa), end(aa), begin(bb), end(bb), actual.end());

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(actual), std::end(actual));
            EXPECT_TRUE(eq);
        }
    }

    void inverse(const std::string& a, const std::string& expected)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_inverse<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_inverse<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);
            auto ex = hex2polynomial<T>(expected);

            std::vector<T> actual(15);
            Cry_inverse<T>(begin(aa), end(aa));

            bool eq = ASSERT_BYTES_EQ(std::begin(ex), std::end(ex), std::begin(aa), std::end(aa));
            EXPECT_TRUE(eq);
        }
    }

    void is_one(const std::string& a)
    {
        {
            using T = uint8_t;
            auto aa = hex2polynomial<T>(a);

            bool f = Cry_is_one<T>(begin(aa), end(aa));

            EXPECT_EQ(f, true);
        }

        {
            using T = uint16_t;
            auto aa = hex2polynomial<T>(a);

            bool f = Cry_is_one<T>(begin(aa), end(aa));

            EXPECT_EQ(f, true);
        }

        {
            using T = uint32_t;
            auto aa = hex2polynomial<T>(a);

            bool f = Cry_is_one<T>(begin(aa), end(aa));

            EXPECT_EQ(f, true);
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
    multiply_1("00000000000000001a03", "00000011", "01BA33");
    multiply_1("00ffff", "0000ffff", "FFFE0001");
    multiply_1("00ff", "00", "00");
    multiply_1("02", "0080", "000100");
    multiply_1("1232AF42", "1232AF42", "14B2AAE35C34D04");
}

TEST_F(BigintCoreTest, Cry_multiply_2)
{
	multiply_2("00000000000000001a03", 0x11, "01BA33");
	multiply_2("0010000000001a03", 0x22, "220000000037466");
	multiply_2("0010000000001a03", 0x33, "330000000052E99");
}

TEST_F(BigintCoreTest, Cry_dev_rem)
{
	div_rem("010107", "02", "8083", "01");
    div_rem("010107", "0003", "0055ad", "000000");
    div_rem("000000010107", "000303", "0055", "000108");
    div_rem("0000161455D642212241", "004315552224414F", "0054", "001155E70E3BB455");
    div_rem("0000161455D642212241", "0024414F", "009BE78DBEAA", "1221CB");
    div_rem("000808", "000009", "0000e4", "000004");
    div_rem("030303", "010203", "000002", "00fefd");
    div_rem("000008", "000004", "000002", "000000");
}

TEST_F(BigintCoreTest, Cry_increment)
{
    increment("000001", "02");
    increment("0001ff", "200");
    increment("01ffff", "20000");
    increment("00fffe", "00ffff");
}

TEST_F(BigintCoreTest, Cry_decrement)
{
    decrement("000002", "000001");
    decrement("000100", "0000ff");
    decrement("000101", "000100");
    decrement("ffffff", "FFFFFE");
}

/*TEST_F(BigintCoreTest, Cry_rotl)
{
        rotl("ff", "ff");
        //rotl("80", "01");
        //rotl("01", "02");
        //rotl("7Fff", "FFFE");
}*/

TEST_F(BigintCoreTest, Cry_rshift)
{
    rshift("123456789", "91A2B3C4");
    rshift("123456789ABCDEF", "91A2B3C4D5E6F7");
    rshift("8000000000000000", "4000000000000000");
}

TEST_F(BigintCoreTest, Cry_lshift)
{
    lshift("123456789", "2468ACF12");
    lshift("123456789ABCDEF", "2468ACF13579BDE");
    lshift("8000000000000000", "00");
}

TEST_F(BigintCoreTest, Cry_and)
{
    f_and("0100000000ff1032", "0100000000ff1031", "0100000000ff1030");
    f_and("0100000000ff1032", "0000ff1031", "0000000000ff1030");
    f_and("01ff4432", "0000ff1031", "0000000000ff0030");
}

TEST_F(BigintCoreTest, Cry_xor)
{
    f_xor("0100000000ff1032", "0100000000ff1032", "00");
    f_xor("0100000000ff1032", "0000000000000000", "0100000000ff1032");
    f_xor("010000ff1032", "0000000000000000", "0000010000ff1032");
    f_xor("030303", "00", "030303");
}

TEST_F(BigintCoreTest, Cry_or)
{
    f_or("0100000000ff1032", "0100000000ff1031", "0100000000ff1033");
    f_or("0100000000ff1032", "0100aa1131", "0100000100ff1133");
    f_or("0116ff1032", "0100000005aa1131", "0100000117ff1133");
    f_or("030303", "00", "030303");
}

TEST_F(BigintCoreTest, Cry_invserse)
{
    inverse("0100000000ff1032", "feffffffff00efcd");
    inverse("0100000000000000", "feffffffffffffff");
}

TEST_F(BigintCoreTest, Cry_is_one)
{
    is_one("0000000000000001");
    is_one("1");
}