
#include "gtest/gtest.h"

#include <iterator>

#include "HugeCore.h"

using namespace std;

static void dump(const byte* f, const byte* l)
{
    for(; f != l; ++f)
    {
        printf("%02x ", *f);
    }
    
    printf("\n");
}

static bool ASSERT_BYTES_EQ(byte* f1, byte* l1, byte* f2, byte* l2)
{
    bool ret = false;

    for (; (f1 != l1) && (*f1 == 0x00); ++f1)
        ;

    for (; (f2 != l2) && (*f2 == 0x00); ++f2)
        ;

    for (; (f1 != l1) && (f2 != l2) && (*f1) == (*f2); ++f1, ++f2)
        ;

    return (f1 == l1) && (f2 == l2);
}

class HugeCore_Test : public ::testing::Test
{
};

TEST(HugeCore_Test, Addition)
{
    {
        bool eq(true);
        byte a[]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x03};
        byte b[]        = {0x00, 0x00, 0x00, 0x11};
        byte expected[] = {0x1a, 0x14};
        byte actual[8]  = {0x00};

        HUGE_Add(end(actual), begin(a), end(a), begin(b), end(b));

        eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }

    // ==============================================================
    {
        byte a[]         = {0x00, 0xff, 0xff};
        byte b[]         = {0x00, 0x00, 0xff, 0xff};
        byte expected2[] = {0x01, 0xFF, 0xFE};
        byte actual[8]   = {0x00};

        HUGE_Add(end(actual), begin(a), end(a), begin(b), end(b));

        auto eq = ASSERT_BYTES_EQ(std::begin(expected2), std::end(expected2), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }

    // ==============================================================
    {
        byte a[]         = {0x00, 0xff};
        byte b[]         = {0x00};
        byte expected3[] = {0xFF};
        byte actual[8]   = {0x00};

        HUGE_Add(end(actual), begin(a), end(a), begin(b), end(b));

        auto eq = ASSERT_BYTES_EQ(std::begin(expected3), std::end(expected3), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }
    {
        byte a[]        = {0x00, 0x00, 0x01, 0xfa, 0x14, 0xba, 0xce, 0x68, 0x02, 0x35};
        byte b[]        = {0x00, 0x00, 0x0a, 0x14, 0x05, 0xf5, 0xef, 0x38, 0x2a, 0x14};
        byte expected[] = {0x0c, 0x0e, 0x1a, 0xb0, 0xbd, 0xa0, 0x2c, 0x49};
        byte actual[16] = {0x00};

        HUGE_Add(end(actual), begin(a), end(a), begin(b), end(b));

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_Multiply_1)
{
    {
        byte a[] = {0x12, 0x02, 0x30};
        byte b   = 0x03;

        byte expected[] = {0x36, 0x06, 0x90};
        byte actual[3]  = {0x00};

        HUGE_Multiply(begin(actual), end(actual), begin(a), end(a), b);

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }

    {
        byte a[] = {0x12, 0x02, 0x30};
        byte b   = 0x11;

        byte expected[] = {0x01, 0x32, 0x25, 0x30};
        byte actual[4]  = {0x00};

        HUGE_Multiply(begin(actual), end(actual), begin(a), end(a), b);

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }

    {
        byte a[] = {0x12, 0x02, 0x30};
        byte b   = 0xfa;

        byte expected[] = {0x11, 0x96, 0x22, 0xE0};
        byte actual[4]  = {0x00};

        HUGE_Multiply(begin(actual), end(actual), begin(a), end(a), b);

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_Multiply)
{
    byte a[]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x03};
    byte b[]        = {0x00, 0x00, 0x00, 0x11};
    byte expected[] = {0x01, 0xBA, 0x33};
    byte actual[8]  = {0x00};

    HUGE_Multiply(begin(actual), end(actual), begin(a), end(a), begin(b), end(b));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a2[]        = {0x00, 0xff, 0xff};
    byte b2[]        = {0x00, 0x00, 0xff, 0xff};
    byte expected2[] = {0xFF, 0xFE, 0x00, 0x01};

    std::fill(std::begin(actual), std::end(actual), 0x00);
    HUGE_Multiply(begin(actual), end(actual), begin(a2), end(a2), begin(b2), end(b2));

    eq = ASSERT_BYTES_EQ(std::begin(expected2), std::end(expected2), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a3[]        = {0x00, 0xff};
    byte b3[]        = {0x00};
    byte expected3[] = {0x00};

    std::fill(std::begin(actual), std::end(actual), 0x00);
    HUGE_Multiply(begin(actual), end(actual), begin(a3), end(a3), begin(b3), end(b3));

    eq = ASSERT_BYTES_EQ(std::begin(expected3), std::end(expected3), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);
    
    {
        byte a[]        = {0x02};
        byte b[]        = {0x00, 0x80};
        byte expected[] = {0x00, 0x01, 0x00};
        byte actual[8]  = {0x00};

        HUGE_Multiply(begin(actual), end(actual), begin(a), end(a), begin(b), end(b));
        //dump(begin(actual), end(actual));

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_Subtract)
{
    byte a[]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01};
    byte b[]        = {0x00, 0x00, 0x00, 0x80};
    byte expected[] = {0x00, 0x81};
    byte actual[8]  = {0x00};

    HUGE_Subtract(end(actual), begin(a), end(a), begin(b), end(b));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    byte a1[] = {
        0x01, 0x01,
    };
    byte b1[] = {
        0x02,
    };
    byte expected1[] = {0xff};
    byte actual1[4]  = {0x00};

    HUGE_Subtract(end(actual1), begin(a1), end(a1), begin(b1), end(b1));

    eq = ASSERT_BYTES_EQ(std::begin(expected1), std::end(expected1), std::begin(actual1), std::end(actual1));

    byte a2[] = {
        0x00, 0x01, 0x00,
    };
    byte b2[]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00};
    byte expected2[] = {0x00};
    byte actual2[4]  = {0x00};

    HUGE_Subtract(end(actual2), begin(a2), end(a2), begin(b2), end(b2));

    eq = ASSERT_BYTES_EQ(std::begin(expected2), std::end(expected2), std::begin(actual2), std::end(actual2));
}

TEST(HugeCore_Test, HUGE_DivRem)
{
    byte a[]            = {0x01, 0x01, 0x07};
    byte b[]            = {0x00, 0x02};
    byte expected_div[] = {0x00, 0x80, 0x83};
    byte expected_rem[] = {0x00, 0x00, 0x01};
    byte actual_div[8]  = {0x00};
    byte actual_rem[8]  = {0x00};

    HUGE_DivRem(begin(actual_div), end(actual_div), begin(actual_rem), end(actual_rem), begin(a), end(a), begin(b), end(b));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected_div), std::end(expected_div), std::begin(actual_div), std::end(actual_div));

    EXPECT_TRUE(eq);

    eq = ASSERT_BYTES_EQ(std::begin(expected_rem), std::end(expected_rem), std::begin(actual_rem), std::end(actual_rem));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a1[]            = {0x01, 0x01, 0x07};
    byte b1[]            = {0x00, 0x03};
    byte expected_div1[] = {0x00, 0x55, 0xad};
    byte expected_rem1[] = {0x00, 0x00, 0x00};
    byte actual_div1[8]  = {0x00};
    byte actual_rem1[8]  = {0x00};

    HUGE_DivRem(begin(actual_div1), end(actual_div1), begin(actual_rem1), end(actual_rem1), begin(a1), end(a1), begin(b1), end(b1));

    eq = ASSERT_BYTES_EQ(std::begin(expected_div1), std::end(expected_div1), std::begin(actual_div1), std::end(actual_div1));

    EXPECT_TRUE(eq);

    eq = ASSERT_BYTES_EQ(std::begin(expected_rem1), std::end(expected_rem1), std::begin(actual_rem1), std::end(actual_rem1));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a2[]            = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x07};
    byte b2[]            = {0x00, 0x03, 0x03};
    byte expected_div2[] = {0x00, 0x55};
    byte expected_rem2[] = {0x00, 0x01, 0x08};
    byte actual_div2[8]  = {0x00};
    byte actual_rem2[8]  = {0x00};

    HUGE_DivRem(begin(actual_div2), end(actual_div2), begin(actual_rem2), end(actual_rem2), begin(a2), end(a2), begin(b2), end(b2));

    eq = ASSERT_BYTES_EQ(std::begin(expected_div2), std::end(expected_div2), std::begin(actual_div2), std::end(actual_div2));

    EXPECT_TRUE(eq);

    eq = ASSERT_BYTES_EQ(std::begin(expected_rem2), std::end(expected_rem2), std::begin(actual_rem2), std::end(actual_rem2));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a3[]            = {0x00, 0x00, 0x16, 0x14, 0x55, 0xD6, 0x42, 0x21, 0x22, 0x41};
    byte b3[]            = {0x00, 0x43, 0x15, 0x55, 0x22, 0x24, 0x41, 0x4F};
    byte expected_div3[] = {0x00, 0x54};
    byte expected_rem3[] = {0x00, 0x11, 0x55, 0xE7, 0x0E, 0x3B, 0xB4, 0x55};
    byte actual_div3[8]  = {0x00};
    byte actual_rem3[8]  = {0x00};

    HUGE_DivRem(begin(actual_div3), end(actual_div3), begin(actual_rem3), end(actual_rem3), begin(a3), end(a3), begin(b3), end(b3));

    eq = ASSERT_BYTES_EQ(std::begin(expected_div3), std::end(expected_div3), std::begin(actual_div3), std::end(actual_div3));

    EXPECT_TRUE(eq);

    eq = ASSERT_BYTES_EQ(std::begin(expected_rem3), std::end(expected_rem3), std::begin(actual_rem3), std::end(actual_rem3));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a4[]            = {0x00, 0x00, 0x16, 0x14, 0x55, 0xD6, 0x42, 0x21, 0x22, 0x41};
    byte b4[]            = {0x00, 0x24, 0x41, 0x4F};
    byte expected_div4[] = {0x00, 0x9B, 0xE7, 0x8D, 0xBE, 0xAA};
    byte expected_rem4[] = {0x12, 0x21, 0xCB};
    byte actual_div4[8]  = {0x00};
    byte actual_rem4[8]  = {0x00};

    HUGE_DivRem(begin(actual_div4), end(actual_div4), begin(actual_rem4), end(actual_rem4), begin(a4), end(a4), begin(b4), end(b4));

    eq = ASSERT_BYTES_EQ(std::begin(expected_div4), std::end(expected_div4), std::begin(actual_div4), std::end(actual_div4));

    EXPECT_TRUE(eq);

    eq = ASSERT_BYTES_EQ(std::begin(expected_rem4), std::end(expected_rem4), std::begin(actual_rem4), std::end(actual_rem4));

    EXPECT_TRUE(eq);
}

TEST(HugeCore_Test, HUGE_Increment)
{
    {
        byte a1[]       = {0x00, 0x00, 0x01};
        byte expected[] = {0x02};

        // byte actual[8] = {0x00};

        HUGE_Increment(begin(a1), end(a1));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a1), end(a1));

        EXPECT_TRUE(eq);
    }
    {
        byte a[]        = {0x00, 0x00, 0xff};
        byte expected[] = {0x00, 0x01, 0x00};

        // byte actual[8] = {0x00};

        HUGE_Increment(begin(a), end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));

        EXPECT_TRUE(eq);
    }
    {
        byte a[]        = {0x00, 0xff, 0xff};
        byte expected[] = {0x01, 0x00, 0x00};

        // byte actual[8] = {0x00};

        HUGE_Increment(begin(a), end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));

        EXPECT_TRUE(eq);
    }
    {
        byte a[]        = {0x00, 0xff, 0xfe};
        byte expected[] = {0x00, 0xff, 0xff};

        // byte actual[8] = {0x00};

        HUGE_Increment(begin(a), end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_Decrement)
{
    {
        byte a[]        = {0x00, 0x00, 0x02};
        byte expected[] = {0x00, 0x00, 0x01};

        // byte actual[8] = {0x00};

        HUGE_Decrement(begin(a), end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));

        EXPECT_TRUE(eq);
    }
    {
        byte a[]        = {0x00, 0x01, 0x00};
        byte expected[] = {0x00, 0x00, 0xff};

        // byte actual[8] = {0x00};

        HUGE_Decrement(begin(a), end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));

        EXPECT_TRUE(eq);
    }
    {
        byte a[]        = {0x00, 0x00, 0x01};
        byte expected[] = {0x00, 0x00, 0x00};

        // byte actual[8] = {0x00};

        HUGE_Decrement(begin(a), end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_Reverse)
{
    {
        byte a[]        = {0x01, 0x02, 0x03};
        byte expected[] = {0x03, 0x02, 0x01};

        HUGE_Reverse(begin(a), end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_And)
{
    {
        byte a[]        = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32};
        byte b[]        = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x31};
        byte expected[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x30};
        byte actual[8]  = {0x00};

        HUGE_And(end(actual), begin(a), end(a), begin(b), end(b));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(actual), end(actual));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_Xor)
{
    {
        byte a[]         = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32};
        byte b[]         = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32};
        byte expected[8] = {0x00};
        byte actual[8]   = {0x00};

        HUGE_Xor(end(actual), begin(a), end(a), begin(b), end(b));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(actual), end(actual));

        EXPECT_TRUE(eq);
    }
    {
        byte a[]        = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32};
        byte b[]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        byte expected[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32};
        byte actual[8]  = {0x00};

        HUGE_Xor(end(actual), begin(a), end(a), begin(b), end(b));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(actual), end(actual));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_Or)
{
    {
        byte a[]        = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32};
        byte b[]        = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x31};
        byte actual[8]  = {0x00};
        byte expected[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x33};

        HUGE_Or(end(actual), begin(a), end(a), begin(b), end(b));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(actual), end(actual));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_Inverse)
{
    {
        byte a[]        = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32};
        byte expected[] = {0xfe, 0xff, 0xff, 0xff, 0xff, 0x00, 0xef, 0xcd};

        HUGE_Inverse(begin(a), end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, HUGE_isOne)
{
    {
        byte a[] = {0x00, 0x00, 0x01};

        bool f = HUGE_isOne(begin(a), end(a));

        EXPECT_TRUE(f);
    }

    {
        byte a[] = {0x01};

        bool f = HUGE_isOne(begin(a), end(a));

        EXPECT_TRUE(f);
    }

    {
        byte a[] = {
            0x00, 0x00, 0x00, 0x00,
        };

        bool f = HUGE_isOne(begin(a), end(a));

        EXPECT_FALSE(f);
    }

    {
        byte a[] = {0xff, 0x00, 0x00, 0x01};

        bool f = HUGE_isOne(begin(a), end(a));

        EXPECT_FALSE(f);
    }

    {
        byte a[] = {0xff};

        bool f = HUGE_isOne(begin(a), end(a));

        EXPECT_FALSE(f);
    }

    {
        byte a[] = {0x00};

        bool f = HUGE_isOne(begin(a), end(a));

        EXPECT_FALSE(f);

        f = HUGE_IsZero(begin(a), end(a));
        EXPECT_TRUE(f);
    }
}
