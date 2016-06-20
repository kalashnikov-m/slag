
#include "gtest/gtest.h"

#include <iterator>

#include "Huge.hpp"

using namespace std;

class HugeTest : public ::testing::Test
{
};

TEST(HugeTest, LogicalNOT)
{
    {
        Huge<byte> a = {0x01, 0x02, 0x03};

        EXPECT_FALSE(!a);
    }

    {
        Huge<byte> a = {0x00, 0x00, 0x00};

        EXPECT_TRUE(!a);
    }
}

TEST(HugeTest, Modulus)
{
    auto ModulusTest = [](const Huge<byte>& arg1, const Huge<byte>& arg2, const Huge<byte>& expected) -> void
    {
        auto mod = arg1 % arg2;

        EXPECT_TRUE(mod == expected);
    };

    ModulusTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
    ModulusTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
    ModulusTest({0x6f, 0x17, 0x23}, {0x17, 0x12}, {0x10, 0x83});
}

TEST(HugeTest, ModulusAssignment)
{
    auto ModulusAssignmentTest = [](Huge<byte>&& arg1, const Huge<byte>& arg2, const Huge<byte>& expected)
    {
        arg1 %= arg2;
        EXPECT_TRUE(arg1 == expected);
    };

    ModulusAssignmentTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
}

TEST(HugeTest, BitwiseAND)
{
    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x01, 0x02, 0x03};

        Huge<byte> c;
        Huge<byte> ex = {0x01, 0x02, 0x03};

        c = (a & b);

        EXPECT_TRUE(c == ex);
    }
}

TEST(HugeTest, BitwiseANDassignment)
{
    auto Test = [](const std::initializer_list<byte>& arg1, const std::initializer_list<byte>& arg2, const std::initializer_list<byte>& arg3) -> void
    {
        Huge<byte> a(arg1);
        Huge<byte> b(arg2);
        Huge<byte> expect(arg3);

        a &= b;

        EXPECT_TRUE(a == expect);
    };

    Test({0x11, 0x22, 0x33}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00});
    Test({0x11, 0x22, 0x33}, {0x11, 0x22, 0x33}, {0x11, 0x22, 0x33});

    {
        Huge<byte> a = {0x11, 0x22, 0x33};
        Huge<byte> b = {0x10, 0x20, 0x30};
        Huge<byte> c = {0x01, 0x02, 0x03};

        Huge<byte> expect = {0x00, 0x00, 0x00};

        (b &= a) &= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, LogicalAND)
{
    auto Test = [](const std::initializer_list<byte>& arg1, const std::initializer_list<byte>& arg2, bool expect) -> void
    {
        Huge<byte> a = arg1;
        Huge<byte> b = arg2;

        EXPECT_TRUE((a && b) == expect);
    };

    Test({0x01, 0x02, 0x03}, {0x01, 0x02, 0x03}, true);
    Test({0x00, 0x00, 0x00}, {0x01, 0x02, 0x03}, false);
    Test({0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, false);
}

TEST(HugeTest, UnaryPlus)
{
    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a == b);
        EXPECT_TRUE(a == +b);
    }

    {
        Huge<byte> a = {0x02, 0x02, 0x03};
        Huge<byte> b = {0x01, 0x02, 0x03};

        EXPECT_FALSE(a == b);
        EXPECT_FALSE(a == +b);
    }
}

TEST(HugeTest, UnaryNegation)
{
    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a == b);

        a = -b;

        EXPECT_TRUE(a != b);

        a = -a;

        EXPECT_TRUE(a == b);
    }
}

TEST(HugeTest, Decrement)
{
    {
        Huge<byte> a      = {0x01, 0x02, 0x03};
        Huge<byte> expect = {0x01, 0x02, 0x02};

        a--;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a      = {0x01, 0x02, 0x03};
        Huge<byte> expect = {0x01, 0x02, 0x02};

        --a;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a = {0x45, 0x38, 0x17};
        Huge<byte> b;
        Huge<byte> expect1 = {0x45, 0x38, 0x17};
        Huge<byte> expect2 = {0x45, 0x38, 0x16};

        b = a--;

        EXPECT_TRUE(b == expect1);
        EXPECT_TRUE(a == expect2);
    }
}

TEST(HugeTest, LogicalOR)
{
    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a || b);
    }

    {
        Huge<byte> a = {0x00, 0x00, 0x00};
        Huge<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a || b);
    }

    {
        Huge<byte> a = {0x00, 0x00, 0x00};
        Huge<byte> b = {0x00, 0x00, 0x00};

        EXPECT_FALSE(a || b);
    }
}

TEST(HugeTest, BitwiseORassignment)
{
    {
        Huge<byte> a = {0x11, 0x22, 0x33};
        Huge<byte> b = {0x00, 0x00, 0x00};

        Huge<byte> expect = {0x11, 0x22, 0x33};

        a |= b;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a = {0x11, 0x22, 0x33};
        Huge<byte> b = {0x10, 0x20, 0x30};
        Huge<byte> c = {0x04, 0x04, 0x04};

        Huge<byte> expect = {0x15, 0x26, 0x37};

        (b |= a) |= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, BitwiseOR)
{
    {
        Huge<byte> a        = {0x01, 0x02, 0x03};
        Huge<byte> b        = {0x11, 0x00, 0x33};
        Huge<byte> expected = {0x11, 0x02, 0x33};

        Huge<byte> c = a | b;

        EXPECT_TRUE(expected == c);
    }
}

TEST(HugeTest, ExclusiveOR)
{
    {
        Huge<byte> a        = {0x01, 0x02, 0x03};
        Huge<byte> b        = {0x11, 0x00, 0x33};
        Huge<byte> expected = {0x10, 0x02, 0x30};

        Huge<byte> c = a ^ b;

        EXPECT_TRUE(expected == c);
    }
}

TEST(HugeTest, ExclusiveORassignment)
{
    {
        Huge<byte> a = {0x11, 0x22, 0x33};
        Huge<byte> b = {0x00, 0x00, 0x00};

        Huge<byte> expect = {0x11, 0x22, 0x33};

        a ^= b;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a = {0x11, 0x22, 0x33};
        Huge<byte> b = {0x10, 0x20, 0x30};
        Huge<byte> c = {0x04, 0x04, 0x04};

        Huge<byte> expect = {0x05, 0x06, 0x07};

        (b ^= a) ^= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, BitwiseInverse)
{
    {
        Huge<byte> a = {0x05};
        Huge<byte> b(~a);

        Huge<byte> ex = {0xFA};

        EXPECT_TRUE(ex == b);
    }

    {
        Huge<byte> a = {0x05};
        Huge<byte> b;

        b = ~a;

        Huge<byte> ex = {0xFA};

        EXPECT_TRUE(ex == b);
    }
}

TEST(HugeTest, Addition)
{
    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x17, 0x20, 0x11};

        Huge<byte> expect = {0x18, 0x22, 0x14};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x17, 0x20, 0x11};

        Huge<byte> expect = {0x16, 0x1e, 0x0e};

        auto c = a + (-b);

        EXPECT_TRUE(c == -expect);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x17, 0x20, 0x11};

        Huge<byte> expect = {0x18, 0x22, 0x14};

        auto c = -a + (-b);

        EXPECT_TRUE(c == -expect);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x17, 0x20, 0x11};

        Huge<byte> expect = {0x16, 0x1e, 0x0e};

        auto c = -a + (b);

        EXPECT_TRUE(c == expect);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03};
        Huge<byte> b = {0x00};

        Huge<byte> expect = {0x01, 0x02, 0x03};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }

    {
        Huge<byte> a = {0x00};
        Huge<byte> b = {0x17, 0x20, 0x11};

        Huge<byte> expect = {0x17, 0x20, 0x11};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }
}

TEST(HugeTest, AdditionAssigment)
{
    {
        Huge<byte> a      = {0x01, 0x02, 0x03};
        Huge<byte> b      = {0x11, 0x27, 0x0a};
        Huge<byte> expect = {0x12, 0x29, 0x0d};

        a += b;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a      = {0x01, 0x02, 0x03};
        Huge<byte> b      = {0x00};
        Huge<byte> expect = {0x01, 0x02, 0x03};

        a += b;

        EXPECT_TRUE(a == expect);
    }
}

TEST(HugeTest, Increment)
{
    {
        Huge<byte> a      = {0x01, 0x02, 0x03};
        Huge<byte> expect = {0x01, 0x02, 0x04};

        a++;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a      = {0x01, 0x02, 0x03};
        Huge<byte> expect = {0x01, 0x02, 0x04};

        ++a;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a = {0x45, 0x38, 0x17};
        Huge<byte> b;
        Huge<byte> expect1 = {0x45, 0x38, 0x17};
        Huge<byte> expect2 = {0x45, 0x38, 0x18};

        b = a++;

        EXPECT_TRUE(b == expect1);
        EXPECT_TRUE(a == expect2);
    }
}

TEST(HugeTest, Subtraction)
{
    { // (a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x00};
        std::initializer_list<byte> il3 = {0x38, 0x22, 0x12};

        Huge<byte> a(il1, true);
        Huge<byte> b(il2);
        Huge<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x22, 0x12};
        std::initializer_list<byte> il3 = {0x38, 0x44, 0x24};

        Huge<byte> a(il1, true);
        Huge<byte> b(il2);
        Huge<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(-b), |a|>|b|
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x17, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x4f, 0x33, 0xab};

        Huge<byte> a(il1);
        Huge<byte> b(il2, true);

        Huge<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(b), |a|<|b|
        std::initializer_list<byte> il1 = {0x22, 0x12};
        std::initializer_list<byte> il2 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x38, 0x44, 0x24};

        Huge<byte> a(il1, true);
        Huge<byte> b(il2);
        Huge<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(-b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x4f, 0x33, 0xab};

        Huge<byte> a(il1);
        Huge<byte> b(il2, true);

        Huge<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    // =========================================================

    { // (-a)-(-b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        Huge<byte> a(il1, true);
        Huge<byte> b(il2, true);

        Huge<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        Huge<byte> a(il1);
        Huge<byte> b(il2);

        Huge<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(-b), |a|>|b|
        std::initializer_list<byte> il1 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        Huge<byte> a(il1, true);
        Huge<byte> b(il2, true);

        Huge<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        Huge<byte> a(il1);
        Huge<byte> b(il2);

        Huge<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }
}

TEST(HugeTest, SubtractionAssigment)
{
    { // (a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x0a};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x04};

        Huge<byte> a(il1);
        Huge<byte> b(il2);

        Huge<byte> expected(il3);

        a -= b;

        EXPECT_TRUE(a == expected);
    }
}

TEST(HugeTest, Multiplication)
{
    {
        std::initializer_list<byte> il1 = {0x0a};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x3c};

        Huge<byte> a(il1);
        Huge<byte> b(il2);
        Huge<byte> expected(il3);

        Huge<byte> c = a * b;
        EXPECT_TRUE(c == expected);
    }

    {
        std::initializer_list<byte> il1 = {0xfa, 0x0a};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x05, 0xdc, 0x3c};

        Huge<byte> a(il1);
        Huge<byte> b(il2);
        Huge<byte> expected(il3);

        Huge<byte> c = a * b;
        EXPECT_TRUE(c == expected);
    }
}

TEST(HugeTest, MultiplicationAssignment)
{
    std::initializer_list<byte> il1 = {0xfa, 0x0a};
    std::initializer_list<byte> il2 = {0x06};
    std::initializer_list<byte> il3 = {0x05, 0xdc, 0x3c};

    Huge<byte> a(il1);
    Huge<byte> b(il2);
    Huge<byte> expected(il3);

    a *= b;

    EXPECT_TRUE(a == expected);
}

TEST(HugeTest, Division)
{
    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x2a, 0xaa};

        Huge<byte> a(il1);
        Huge<byte> b(il2);
        Huge<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0xf6, 0xf5};
        std::initializer_list<byte> il3 = {0x01};

        Huge<byte> a(il1);
        Huge<byte> b(il2);
        Huge<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0xff, 0xf1, 0xff, 0xfa, 0x56, 0x28, 0xff};
        std::initializer_list<byte> il2 = {0xf6, 0xf5, 0x03, 17};
        std::initializer_list<byte> il3 = {0x01, 0x09, 0x51, 0x3e};

        Huge<byte> a(il1);
        Huge<byte> b(il2);
        Huge<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0x6f, 0x17, 0x23};
        std::initializer_list<byte> il2 = {0x17, 0x12};
        std::initializer_list<byte> il3 = {0x04, 0xd0};

        Huge<byte> a(il1);
        Huge<byte> b(il2, true);
        Huge<byte> expected_div(il3, true);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }
}

TEST(HugeTest, DivisionAssignment)
{
    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x2a, 0xaa};

        Huge<byte> a(il1);
        Huge<byte> b(il2);
        Huge<byte> expected_div(il3);

        a /= b;

        EXPECT_TRUE(a == expected_div);
    }
}

TEST(HugeTest, LeftShift)
{
    {
        Huge<byte> a = {0x22, 0x12};
        Huge<byte> b(a << 1);
        Huge<byte> expect = {0x44, 0x24};

        EXPECT_TRUE(expect == b);
    }

    {
        Huge<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        Huge<byte> c(a << 2);
        Huge<byte> expect = {0xC8, 0x18, 0x48, 0x88, 0x48};

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, LeftShiftAssigment)
{
    {
        Huge<byte> a = {0x22, 0x12};

        a <<= 1;

        Huge<byte> expect = {0x44, 0x24};

        EXPECT_TRUE(expect == a);
    }

    {
        Huge<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        Huge<byte> c(a << 2);
        Huge<byte> expect = {0xC8, 0x18, 0x48, 0x88, 0x48};

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, RightShift)
{
    {
        Huge<byte> a = {0x22, 0x12};
        Huge<byte> b(a >> 1);
        Huge<byte> expect = {0x11, 0x09};

        EXPECT_TRUE(expect == b);
    }

    {
        Huge<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        Huge<byte> c(a >> 2);
        Huge<byte> expect = {0x0C, 0x81, 0x84, 0x88, 0x84};

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, RightShiftAssignment)
{
    {
        Huge<byte> a = {0x22, 0x12};

        a >>= 1;

        Huge<byte> expect = {0x11, 0x09};

        EXPECT_TRUE(expect == a);
    }

    {
        Huge<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};

        a >>= 2;

        Huge<byte> expect = {0x0C, 0x81, 0x84, 0x88, 0x84};

        EXPECT_TRUE(expect == a);
    }
}

TEST(HugeTest, Less)
{
    {
        Huge<byte> a = {0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a < b);
    }

    {
        Huge<byte> a = {0x38, 0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a < b);
    }

    {
        Huge<byte> a = {0x21, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a < b);
    }

    {
        Huge<byte> a = {0x00, 0x00};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a < b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a < b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a < b);
    }
}

TEST(HugeTest, LessEqual)
{
    {
        Huge<byte> a = {0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x22, 0x12};

        Huge<byte> a(il1);
        Huge<byte> b(il2, true);

        EXPECT_FALSE(a <= b);
    }

    {
        Huge<byte> a = {0x21, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        Huge<byte> a = {0x00, 0x00};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a <= b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a <= b);
    }

    {
        std::initializer_list<byte> il1 = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        std::initializer_list<byte> il2 = {0x01, 0x02, 0x03, 0x18, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        Huge<byte> a(il1, true);
        Huge<byte> b(il2);

        EXPECT_TRUE(a <= b);
    }
}

TEST(HugeTest, Greather)
{
    {
        Huge<byte> a = {0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        Huge<byte> a = {0x38, 0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a > b);
    }

    {
        Huge<byte> a = {0x21, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        Huge<byte> a = {0x00, 0x00};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a > b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a > b);
    }
}

TEST(HugeTest, GreatherEqual)
{
    {
        Huge<byte> a = {0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a >= b);
    }

    {
        Huge<byte> a = {0x38, 0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a >= b);
    }

    {
        Huge<byte> a = {0x21, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a >= b);
    }

    {
        Huge<byte> a = {0x00, 0x00};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a >= b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a >= b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a >= b);
    }
}

TEST(HugeTest, Equal)
{
    {
        Huge<byte> a = {0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a == b);
    }

    {
        Huge<byte> a = {0x38, 0x22, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        Huge<byte> a = {0x21, 0x12};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        Huge<byte> a = {0x00, 0x00};
        Huge<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a == b);
    }

    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        Huge<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a == b);
    }

    std::initializer_list<byte> il = {0x01, 0x02};

    {
        Huge<byte> a(il, true); // -258
        Huge<byte> b(il, true); // -258

        EXPECT_TRUE(a == b);
    }

    {
        Huge<byte> a(il, false); // 258
        Huge<byte> b(il, true);  // -258

        EXPECT_FALSE(a == b);
    }
}

TEST(HugeTest, Inequality)
{
    auto InequalityTest = [](const Huge<byte>& arg1, const Huge<byte>& arg2, bool expected)
    {
        bool eq = arg1 != arg2;
        EXPECT_TRUE(eq == expected);
    };

    InequalityTest({0x22, 0x12}, {0x22, 0x12}, false);
    InequalityTest({0x38, 0x22, 0x12}, {0x22, 0x12}, true);
    InequalityTest({0x21, 0x12}, {0x22, 0x12}, true);
    InequalityTest({0x00, 0x00}, {0x22, 0x12}, true);
    InequalityTest({0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c}, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c}, false);
    InequalityTest({0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c}, {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c}, true);
}

TEST(HugeTest, BOOL)
{
    {
        Huge<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        bool f = (bool)a;

        EXPECT_TRUE(f);
    }

    {
        Huge<byte> a = {0x00, 0x00, 0x00};

        bool f = (bool)a;

        EXPECT_FALSE(f);
    }
}

TEST(HugeTest, Gcd)
{
    {
        Huge<byte> a      = {0x04, 0x2f};       // 1071
        Huge<byte> b      = {0x01, 0xce};       // 462
        Huge<byte> nod    = {0x15};             // 21
        Huge<byte> actual = {0x00, 0x00, 0x00}; //

        actual = a.Gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }

    {
        Huge<byte> a      = {18};   //
        Huge<byte> b      = {30};   //
        Huge<byte> nod    = {6};    //
        Huge<byte> actual = {0x00}; //

        actual = a.Gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }

    {
        Huge<byte> a      = {0x6b, 0x7d}; //
        Huge<byte> b      = {0x31, 0x4a}; //
        Huge<byte> nod    = {0x01};       //
        Huge<byte> actual = {0x00, 0x00}; //

        actual = a.Gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }
}

TEST(HugeTest, ModInvserse)
{
    {
        Huge<byte> a   = {0x1b};       // 27
        Huge<byte> N   = {0x04, 0x09}; // 1033
        Huge<byte> inv = {0x03, 0x70}; // 880
        Huge<byte> actual;

        bool exists = a.ModInverse(actual, N);

        EXPECT_TRUE(exists);
        bool f = (actual == inv);

        EXPECT_TRUE(f);
    }

    {
        Huge<byte> a   = {0x01, 0xf3};       // 499
        Huge<byte> N   = {0x92, 0xbf, 0xb0}; // 9617328
        Huge<byte> inv = {0x7a, 0x0b, 0xab}; // 7998379
        Huge<byte> actual;

        bool exists = a.ModInverse(actual, N);

        EXPECT_TRUE(exists);
        bool f = (actual == inv);

        EXPECT_TRUE(f);
    }

    {
        Huge<byte> a   = {0x03};             // 3
        Huge<byte> N   = {0x8B, 0xF9, 0xFF}; // 9177503
        Huge<byte> inv = {0x5d, 0x41, 0x5b}; // 6111579
        Huge<byte> actual;

        bool exists = a.ModInverse(actual, N);

        EXPECT_TRUE(exists);
        bool f = (actual == inv);

        EXPECT_TRUE(f);
    }
}

TEST(HugeTest, DivRem)
{
    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x2a, 0xaa};

        Huge<byte> a(il1);
        Huge<byte> b(il2);
        Huge<byte> expected_div(il3);
        Huge<byte> expected_rem({0x03});

        Huge<byte> q;
        Huge<byte> r;

        a.DivRem(q, r, b);

        EXPECT_TRUE(q == expected_div);
        EXPECT_TRUE(r == expected_rem);
    }

    {
        Huge<byte> a({0x10, 0x22, 0xd2, 0x74, 0x02, 0x72, 0x19, 0x2d});
        Huge<byte> b({0x98, 0x66, 0x06, 0x3b});
        Huge<byte> expected_div({0x1b, 0x1a, 0xfb, 0x05});
        Huge<byte> expected_rem({0x25, 0x5a, 0x21, 0x06});

        Huge<byte> q;
        Huge<byte> r;

        a.DivRem(q, r, b);

        EXPECT_TRUE(q == expected_div);
        EXPECT_TRUE(r == expected_rem);
    }
}
