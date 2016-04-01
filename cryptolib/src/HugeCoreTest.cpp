
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
    auto AdditionTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected) -> void
    {
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
    auto Multiply1Test = [](const std::initializer_list<byte>& a, byte b, const std::initializer_list<byte>& expected) -> void
    {
        byte actual[8] = {0x00};

        HUGE_Multiply(begin(actual), end(actual), begin(a), end(a), b);

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));
        EXPECT_TRUE(eq);
    };

    Multiply1Test({0x12, 0x02, 0x30}, 0x03, {0x36, 0x06, 0x90});
    Multiply1Test({0x12, 0x02, 0x30}, 0x11, {0x01, 0x32, 0x25, 0x30});
    Multiply1Test({0x12, 0x02, 0x30}, 0xfa, {0x11, 0x96, 0x22, 0xE0});
}

TEST(HugeCore_Test, Multiply)
{
    auto MultiplyTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected) -> void
    {
        byte actual[8] = {0x00};
        HUGE_Multiply(begin(actual), end(actual), begin(a), end(a), begin(b), end(b));

        bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

        EXPECT_TRUE(eq);
    };

    MultiplyTest({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x03}, {0x00, 0x00, 0x00, 0x11}, {0x01, 0xBA, 0x33});
    MultiplyTest({0x00, 0xff, 0xff}, {0x00, 0x00, 0xff, 0xff}, {0xFF, 0xFE, 0x00, 0x01});
    MultiplyTest({0x00, 0xff}, {0x00}, {0x00});
    MultiplyTest({0x02}, {0x00, 0x80}, {0x00, 0x01, 0x00});
}

TEST(HugeCore_Test, Subtract)
{
    auto SubtractTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected) -> void
    {
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
    auto DivRemTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected_div, const std::initializer_list<byte>& expected_rem) -> void
    {
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

TEST(HugeCore_Test, Increment)
{
    auto IncrementTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& expected) -> void
    {
        HUGE_Increment((byte*)begin(a), (byte*)end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));
        EXPECT_TRUE(eq);
    };

    IncrementTest({0x00, 0x00, 0x01}, {0x02});
    IncrementTest({0x00, 0x00, 0xff}, {0x00, 0x01, 0x00});
    IncrementTest({0x00, 0xff, 0xff}, {0x01, 0x00, 0x00});
    IncrementTest({0x00, 0xff, 0xfe}, {0x00, 0xff, 0xff});
}

TEST(HugeCore_Test, Decrement)
{
    auto DecrementTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& expected) -> void
    {

        HUGE_Decrement((byte*)begin(a), (byte*)end(a));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(a), end(a));
        EXPECT_TRUE(eq);
    };

    DecrementTest({0x00, 0x00, 0x02}, {0x00, 0x00, 0x01});
    DecrementTest({0x00, 0x01, 0x00}, {0x00, 0x00, 0xff});
    DecrementTest({0x00, 0x01, 0x1}, {0x00, 0x01, 0x00});
}

TEST(HugeCore_Test, Reverse)
{
    auto ReverseTest = [](const std::initializer_list<byte>& arg, const std::initializer_list<byte>& expected)
    {
        HUGE_Reverse((byte*)begin(arg), (byte*)end(arg));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(arg), end(arg));

        EXPECT_TRUE(eq);
    };

    ReverseTest({0x01, 0x02, 0x03}, {0x03, 0x02, 0x01});
}

TEST(HugeCore_Test, And)
{
    auto AndTest = [](const std::initializer_list<byte>& arg1, const std::initializer_list<byte>& arg2, const std::initializer_list<byte>& expected) -> void
    {
        byte actual[8] = {0x00};
        HUGE_And((byte*)end(actual), begin(arg1), end(arg1), begin(arg2), end(arg2));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(actual), end(actual));

        EXPECT_TRUE(eq);
    };

    AndTest({0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32}, {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x31}, {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x30});
}

TEST(HugeCore_Test, Xor)
{
    auto XorTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected) -> void
    {
        byte actual[8] = {0x00};
        HUGE_Xor(end(actual), begin(a), end(a), begin(b), end(b));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(actual), end(actual));

        EXPECT_TRUE(eq);
    };

    XorTest({0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32}, {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32}, {0x00});
    XorTest({0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32});
}

TEST(HugeCore_Test, Or)
{
    auto OrTest = [](const std::initializer_list<byte>& a, const std::initializer_list<byte>& b, const std::initializer_list<byte>& expected) -> void
    {
        byte actual[8] = {0x00};
        HUGE_Or(end(actual), begin(a), end(a), begin(b), end(b));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(actual), end(actual));

        EXPECT_TRUE(eq);
    };

    OrTest({0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32}, {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x31}, {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x33});
}

TEST(HugeCore_Test, Inverse)
{
    auto InverseTest = [](const std::initializer_list<byte>& arg, const std::initializer_list<byte>& expected) -> void
    {
        HUGE_Inverse((byte*)begin(arg), (byte*)end(arg));

        auto eq = ASSERT_BYTES_EQ(begin(expected), end(expected), begin(arg), end(arg));

        EXPECT_TRUE(eq);
    };

    InverseTest({0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x32}, {0xfe, 0xff, 0xff, 0xff, 0xff, 0x00, 0xef, 0xcd});
}

TEST(HugeCore_Test, isOne)
{
    auto IsOneTest = [](const std::initializer_list<byte>& arg, bool isOne) -> void
    {
        bool f = HUGE_IsOne(begin(arg), end(arg));

        EXPECT_TRUE(f == isOne);
    };

    IsOneTest({0x01}, true);
    IsOneTest({0xff}, false);
    IsOneTest({0x00}, false);
    IsOneTest({0x00, 0x00, 0x01}, true);
    IsOneTest({0x00, 0x00, 0x00, 0x00}, false);
    IsOneTest({0xff, 0x00, 0x00, 0x01}, false);
}
