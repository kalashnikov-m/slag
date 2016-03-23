
#include "gtest/gtest.h"

#include <iterator>

#include "HugeCore.h"

using namespace std;

static void dump(const byte* f, const byte* l)
{
    for (; f != l; ++f)
    {
        printf("%02x ", *f);
    }

    printf("\n");
}

static bool ASSERT_BYTES_EQ(const byte* f1, const byte* l1, const byte* f2, const byte* l2)
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
    auto AdditionTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected) -> void {
        byte actual[8] = {0x00};

        HUGE_Add(end(actual), begin(a), end(a), begin(b), end(b));

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));
        EXPECT_TRUE(eq);
    };

    AdditionTest({0x00, 0xff}, {0x00}, {0xff});
    AdditionTest({0x00, 0x1a, 0x03}, {0x00, 0x00, 0x11}, {0x1a, 0x14});
    AdditionTest({0x00, 0xff, 0xff}, {0x00, 0x00, 0xff, 0xff}, {0x01, 0xFF, 0xFE});
    AdditionTest({0x00, 0x00, 0x01, 0xfa, 0x14, 0xba, 0xce, 0x68, 0x02, 0x35}, {0x00, 0x00, 0x0a, 0x14, 0x05, 0xf5, 0xef, 0x38, 0x2a, 0x14}, {0x0c, 0x0e, 0x1a, 0xb0, 0xbd, 0xa0, 0x2c, 0x49});
}

TEST(HugeCore_Test, Multiply_1)
{
    auto Multiply1Test = [](const std::initializer_list<byte>& a, byte b, const std::initializer_list<byte>& expected) -> void {
        byte actual[8] = {0x00};

        HUGE_Multiply(begin(actual), end(actual), begin(a), end(a), b);

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));
        EXPECT_TRUE(eq);
    };

    Multiply1Test({0x12, 0x02, 0x30}, 0x03, {0x36, 0x06, 0x90});
    Multiply1Test({0x12, 0x02, 0x30}, 0x11, {0x01, 0x32, 0x25, 0x30});
    Multiply1Test({0x12, 0x02, 0x30}, 0xfa, {0x11, 0x96, 0x22, 0xE0});
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

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    }
}

TEST(HugeCore_Test, Subtract)
{
    auto SubtractTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected) -> void {
        byte actual[8] = {0x00};

        HUGE_Subtract(end(actual), begin(a), end(a), begin(b), end(b));

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));
        EXPECT_TRUE(eq);
    };

    SubtractTest({0x01, 0x01}, {0x02}, {0xff});
    SubtractTest({0x00, 0x01, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00}, {0x00});
    SubtractTest({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01}, {0x00, 0x00, 0x00, 0x80}, {0x00, 0x81});
}

TEST(HugeCore_Test, DivRem)
{
    auto DivRemTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected_div, const std::initializer_list<byte>& expected_rem) -> void {
        byte div[8] = {0x00};
        byte rem[8] = {0x00};

        HUGE_DivRem(begin(div), end(div), begin(rem), end(rem), begin(a), end(a), begin(b), end(b));

        bool eq = ASSERT_BYTES_EQ(begin(expected_div), end(expected_div), begin(div), end(div));
        EXPECT_TRUE(eq);

        eq = ASSERT_BYTES_EQ(begin(expected_rem), end(expected_rem), begin(rem), end(rem));
        EXPECT_TRUE(eq);
    };

    DivRemTest({0x01, 0x01, 0x07}, {0x00, 0x02}, {0x00, 0x80, 0x83}, {0x00, 0x00, 0x01});
    DivRemTest({0x01, 0x01, 0x07}, {0x00, 0x03}, {0x00, 0x55, 0xad}, {0x00, 0x00, 0x00});
    DivRemTest({0x00, 0x00, 0x00, 0x01, 0x01, 0x07}, {0x00, 0x03, 0x03}, {0x00, 0x55}, {0x00, 0x01, 0x08});
    DivRemTest({0x00, 0x00, 0x16, 0x14, 0x55, 0xD6, 0x42, 0x21, 0x22, 0x41}, {0x00, 0x43, 0x15, 0x55, 0x22, 0x24, 0x41, 0x4F}, {0x00, 0x54}, {0x00, 0x11, 0x55, 0xE7, 0x0E, 0x3B, 0xB4, 0x55});
    DivRemTest({0x00, 0x00, 0x16, 0x14, 0x55, 0xD6, 0x42, 0x21, 0x22, 0x41}, {0x00, 0x24, 0x41, 0x4F}, {0x00, 0x9B, 0xE7, 0x8D, 0xBE, 0xAA}, {0x12, 0x21, 0xCB});
    DivRemTest({0x00, 0x08, 0x08}, {0x00, 0x00, 0x09}, {0x00, 0x00, 0xe4}, {0x00, 0x00, 0x04});
    DivRemTest({0x03, 0x03, 0x03}, {0x01, 0x02, 0x03}, {0x00, 0x00, 0x02}, {0x00, 0xfe, 0xfd});
    DivRemTest({0x00, 0x00, 0x08}, {0x00, 0x00, 0x04}, {0x00, 0x00, 0x02}, {0x00, 0x00, 0x00});
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

TEST(HugeCore_Test, isOne)
{
    auto IsOneTest = [](const std::initializer_list<byte>& arg, bool isOne) -> void {
        bool f = HUGE_isOne(begin(arg), end(arg));

        EXPECT_TRUE(f == isOne);
    };

    IsOneTest({0x01}, true);
    IsOneTest({0xff}, false);
    IsOneTest({0x00}, false);
    IsOneTest({0x00, 0x00, 0x01}, true);
    IsOneTest({0x00, 0x00, 0x00, 0x00}, false);
    IsOneTest({0xff, 0x00, 0x00, 0x01}, false);
}
