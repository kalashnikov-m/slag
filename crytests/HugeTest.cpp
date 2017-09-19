
#include "gtest/gtest.h"

#include "algorithm.hpp"

#include "basic_int.hpp"
#include "OS2IP.hpp"

using namespace std;
using namespace cry;

namespace
{
    template <class InputIterator>
    bool ASSERT_BYTES_EQ(InputIterator f1, InputIterator l1, InputIterator f2, InputIterator l2)
    {

        for (; (f1 != l1) && (*f1 == 0x00); ++f1)
            ;

        for (; (f2 != l2) && (*f2 == 0x00); ++f2)
            ;

        for (; (f1 != l1) && (f2 != l2) && (*f1) == (*f2); ++f1, ++f2)
            ;

        return (f1 == l1) && (f2 == l2);
    }
}

class HugeTest : public ::testing::Test
{
};

using byte = uint8_t;

TEST(HugeTest, LogicalNOT)
{
    {
        basic_int<byte> a = {0x01, 0x02, 0x03};

        EXPECT_FALSE(!a);
    }

    {
        basic_int<byte> a = {0x00, 0x00, 0x00};

        EXPECT_TRUE(!a);
    }
}

TEST(HugeTest, Modulus)
{
    auto ModulusTest = [](const basic_int<byte>& arg1, const basic_int<byte>& arg2, const basic_int<byte>& expected) -> void {
        auto mod = arg1 % arg2;

        EXPECT_TRUE(mod == expected);
    };

    ModulusTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
    ModulusTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
    ModulusTest({0x6f, 0x17, 0x23}, {0x17, 0x12}, {0x10, 0x83});
}

TEST(HugeTest, ModulusAssignment)
{
    auto ModulusAssignmentTest = [](basic_int<byte>&& arg1, const basic_int<byte>& arg2, const basic_int<byte>& expected) {
        arg1 %= arg2;
        EXPECT_TRUE(arg1 == expected);
    };

    ModulusAssignmentTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
}

TEST(HugeTest, BitwiseAND)
{
    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x01, 0x02, 0x03};

        basic_int<byte> c;
        basic_int<byte> ex = {0x01, 0x02, 0x03};

        c = (a & b);

        EXPECT_TRUE(c == ex);
    }
}

TEST(HugeTest, BitwiseANDassignment)
{
    auto Test = [](const std::initializer_list<byte>& arg1, const std::initializer_list<byte>& arg2, const std::initializer_list<byte>& arg3) -> void {
        basic_int<byte> a(arg1);
        basic_int<byte> b(arg2);
        basic_int<byte> expect(arg3);

        a &= b;

        EXPECT_TRUE(a == expect);
    };

    Test({0x11, 0x22, 0x33}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00});
    Test({0x11, 0x22, 0x33}, {0x11, 0x22, 0x33}, {0x11, 0x22, 0x33});

    {
        basic_int<byte> a = {0x11, 0x22, 0x33};
        basic_int<byte> b = {0x10, 0x20, 0x30};
        basic_int<byte> c = {0x01, 0x02, 0x03};

        basic_int<byte> expect = {0x00, 0x00, 0x00};

        (b &= a) &= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, LogicalAND)
{
    auto Test = [](const std::initializer_list<byte>& arg1, const std::initializer_list<byte>& arg2, bool expect) -> void {
        basic_int<byte> a = arg1;
        basic_int<byte> b = arg2;

        EXPECT_TRUE((a && b) == expect);
    };

    Test({0x01, 0x02, 0x03}, {0x01, 0x02, 0x03}, true);
    Test({0x00, 0x00, 0x00}, {0x01, 0x02, 0x03}, false);
    Test({0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, false);
}

TEST(HugeTest, UnaryPlus)
{
    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a == b);
        EXPECT_TRUE(a == +b);
    }

    {
        basic_int<byte> a = {0x02, 0x02, 0x03};
        basic_int<byte> b = {0x01, 0x02, 0x03};

        EXPECT_FALSE(a == b);
        EXPECT_FALSE(a == +b);
    }
}

TEST(HugeTest, UnaryNegation)
{
    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x01, 0x02, 0x03};

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
        basic_int<byte> a      = {0x01, 0x02, 0x03};
        basic_int<byte> expect = {0x01, 0x02, 0x02};

        a--;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_int<byte> a      = {0x01, 0x02, 0x03};
        basic_int<byte> expect = {0x01, 0x02, 0x02};

        --a;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_int<byte> a = {0x45, 0x38, 0x17};
        basic_int<byte> b;
        basic_int<byte> expect1 = {0x45, 0x38, 0x17};
        basic_int<byte> expect2 = {0x45, 0x38, 0x16};

        b = a--;

        EXPECT_TRUE(b == expect1);
        EXPECT_TRUE(a == expect2);
    }
}

TEST(HugeTest, LogicalOR)
{
    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a || b);
    }

    {
        basic_int<byte> a = {0x00, 0x00, 0x00};
        basic_int<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a || b);
    }

    {
        basic_int<byte> a = {0x00, 0x00, 0x00};
        basic_int<byte> b = {0x00, 0x00, 0x00};

        EXPECT_FALSE(a || b);
    }
}

TEST(HugeTest, BitwiseORassignment)
{
    {
        basic_int<byte> a = {0x11, 0x22, 0x33};
        basic_int<byte> b = {0x00, 0x00, 0x00};

        basic_int<byte> expect = {0x11, 0x22, 0x33};

        a |= b;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_int<byte> a = {0x11, 0x22, 0x33};
        basic_int<byte> b = {0x10, 0x20, 0x30};
        basic_int<byte> c = {0x04, 0x04, 0x04};

        basic_int<byte> expect = {0x15, 0x26, 0x37};

        (b |= a) |= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, BitwiseOR)
{
    {
        basic_int<byte> a        = {0x01, 0x02, 0x03};
        basic_int<byte> b        = {0x11, 0x00, 0x33};
        basic_int<byte> expected = {0x11, 0x02, 0x33};

        basic_int<byte> c = a | b;

        EXPECT_TRUE(expected == c);
    }
}

TEST(HugeTest, ExclusiveOR)
{
    {
        basic_int<byte> a        = {0x01, 0x02, 0x03};
        basic_int<byte> b        = {0x11, 0x00, 0x33};
        basic_int<byte> expected = {0x10, 0x02, 0x30};

        basic_int<byte> c = a ^ b;

        EXPECT_TRUE(expected == c);
    }
}

TEST(HugeTest, ExclusiveORassignment)
{
    {
        basic_int<byte> a = {0x11, 0x22, 0x33};
        basic_int<byte> b = {0x00, 0x00, 0x00};

        basic_int<byte> expect = {0x11, 0x22, 0x33};

        a ^= b;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_int<byte> a = {0x11, 0x22, 0x33};
        basic_int<byte> b = {0x10, 0x20, 0x30};
        basic_int<byte> c = {0x04, 0x04, 0x04};

        basic_int<byte> expect = {0x05, 0x06, 0x07};

        (b ^= a) ^= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, BitwiseInverse)
{
    {
        basic_int<byte> a = {0x05};
        basic_int<byte> b(~a);

        basic_int<byte> ex = {0xFA};

        EXPECT_TRUE(ex == b);
    }

    {
        basic_int<byte> a = {0x05};
        basic_int<byte> b;

        b = ~a;

        basic_int<byte> ex = {0xFA};

        EXPECT_TRUE(ex == b);
    }
}

TEST(HugeTest, Addition)
{
    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x17, 0x20, 0x11};

        basic_int<byte> expect = {0x18, 0x22, 0x14};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x17, 0x20, 0x11};

        basic_int<byte> expect = {0x16, 0x1e, 0x0e};

        auto c = a + (-b);

        EXPECT_TRUE(c == -expect);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x17, 0x20, 0x11};

        basic_int<byte> expect = {0x18, 0x22, 0x14};

        auto c = -a + (-b);

        EXPECT_TRUE(c == -expect);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x17, 0x20, 0x11};

        basic_int<byte> expect = {0x16, 0x1e, 0x0e};

        auto c = -a + (b);

        EXPECT_TRUE(c == expect);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03};
        basic_int<byte> b = {0x00};

        basic_int<byte> expect = {0x01, 0x02, 0x03};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }

    {
        basic_int<byte> a = {0x00};
        basic_int<byte> b = {0x17, 0x20, 0x11};

        basic_int<byte> expect = {0x17, 0x20, 0x11};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }
}

TEST(HugeTest, AdditionAssigment)
{
    {
        basic_int<byte> a      = {0x01, 0x02, 0x03};
        basic_int<byte> b      = {0x11, 0x27, 0x0a};
        basic_int<byte> expect = {0x12, 0x29, 0x0d};

        a += b;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_int<byte> a      = {0x01, 0x02, 0x03};
        basic_int<byte> b      = {0x00};
        basic_int<byte> expect = {0x01, 0x02, 0x03};

        a += b;

        EXPECT_TRUE(a == expect);
    }
}

TEST(HugeTest, Increment)
{
    {
        basic_int<byte> a      = {0x01, 0x02, 0x03};
        basic_int<byte> expect = {0x01, 0x02, 0x04};

        a++;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_int<byte> a      = {0x01, 0x02, 0x03};
        basic_int<byte> expect = {0x01, 0x02, 0x04};

        ++a;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_int<byte> a = {0x45, 0x38, 0x17};
        basic_int<byte> b;
        basic_int<byte> expect1 = {0x45, 0x38, 0x17};
        basic_int<byte> expect2 = {0x45, 0x38, 0x18};

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

        basic_int<byte> a(il1, true);
        basic_int<byte> b(il2);
        basic_int<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x22, 0x12};
        std::initializer_list<byte> il3 = {0x38, 0x44, 0x24};

        basic_int<byte> a(il1, true);
        basic_int<byte> b(il2);
        basic_int<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(-b), |a|>|b|
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x17, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x4f, 0x33, 0xab};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2, true);

        basic_int<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(b), |a|<|b|
        std::initializer_list<byte> il1 = {0x22, 0x12};
        std::initializer_list<byte> il2 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x38, 0x44, 0x24};

        basic_int<byte> a(il1, true);
        basic_int<byte> b(il2);
        basic_int<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(-b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x4f, 0x33, 0xab};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2, true);

        basic_int<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    // =========================================================

    { // (-a)-(-b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        basic_int<byte> a(il1, true);
        basic_int<byte> b(il2, true);

        basic_int<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);

        basic_int<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(-b), |a|>|b|
        std::initializer_list<byte> il1 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        basic_int<byte> a(il1, true);
        basic_int<byte> b(il2, true);

        basic_int<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);

        basic_int<byte> expected(il3);

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

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);

        basic_int<byte> expected(il3);

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

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);
        basic_int<byte> expected(il3);

        basic_int<byte> c = a * b;
        EXPECT_TRUE(c == expected);
    }

    {
        std::initializer_list<byte> il1 = {0xfa, 0x0a};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x05, 0xdc, 0x3c};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);
        basic_int<byte> expected(il3);

        basic_int<byte> c = a * b;
        EXPECT_TRUE(c == expected);
    }
}

TEST(HugeTest, MultiplicationAssignment)
{
    std::initializer_list<byte> il1 = {0xfa, 0x0a};
    std::initializer_list<byte> il2 = {0x06};
    std::initializer_list<byte> il3 = {0x05, 0xdc, 0x3c};

    basic_int<byte> a(il1);
    basic_int<byte> b(il2);
    basic_int<byte> expected(il3);

    a *= b;

    EXPECT_TRUE(a == expected);
}

TEST(HugeTest, Division)
{
    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x2a, 0xaa};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);
        basic_int<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0xf6, 0xf5};
        std::initializer_list<byte> il3 = {0x01};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);
        basic_int<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0xff, 0xf1, 0xff, 0xfa, 0x56, 0x28, 0xff};
        std::initializer_list<byte> il2 = {0xf6, 0xf5, 0x03, 17};
        std::initializer_list<byte> il3 = {0x01, 0x09, 0x51, 0x3e};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);
        basic_int<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0x6f, 0x17, 0x23};
        std::initializer_list<byte> il2 = {0x17, 0x12};
        std::initializer_list<byte> il3 = {0x04, 0xd0};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2, true);
        basic_int<byte> expected_div(il3, true);

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

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);
        basic_int<byte> expected_div(il3);

        a /= b;

        EXPECT_TRUE(a == expected_div);
    }
}

TEST(HugeTest, LeftShift)
{
    {
        basic_int<byte> a = {0x22, 0x12};
        basic_int<byte> b(a << 1);
        basic_int<byte> expect = {0x44, 0x24};

        EXPECT_TRUE(expect == b);
    }

    {
        basic_int<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        basic_int<byte> c(a << 2);
        basic_int<byte> expect = {0xC8, 0x18, 0x48, 0x88, 0x48};

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, LeftShiftAssigment)
{
    {
        basic_int<byte> a = {0x22, 0x12};

        a <<= 1;

        basic_int<byte> expect = {0x44, 0x24};

        EXPECT_TRUE(expect == a);
    }

    {
        basic_int<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        basic_int<byte> c(a << 2);
        basic_int<byte> expect = {0xC8, 0x18, 0x48, 0x88, 0x48};

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, RightShift)
{
    {
        basic_int<byte> a = {0x22, 0x12};
        basic_int<byte> b(a >> 1);
        basic_int<byte> expect = {0x11, 0x09};

        EXPECT_TRUE(expect == b);
    }

    {
        basic_int<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        basic_int<byte> c(a >> 2);
        basic_int<byte> expect = {0x0C, 0x81, 0x84, 0x88, 0x84};

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, RightShiftAssignment)
{
    {
        basic_int<byte> a = {0x22, 0x12};

        a >>= 1;

        basic_int<byte> expect = {0x11, 0x09};

        EXPECT_TRUE(expect == a);
    }

    {
        basic_int<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};

        a >>= 2;

        basic_int<byte> expect = {0x0C, 0x81, 0x84, 0x88, 0x84};

        EXPECT_TRUE(expect == a);
    }
}

TEST(HugeTest, Less)
{
    {
        basic_int<byte> a = {0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a < b);
    }

    {
        basic_int<byte> a = {0x38, 0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a < b);
    }

    {
        basic_int<byte> a = {0x21, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a < b);
    }

    {
        basic_int<byte> a = {0x00, 0x00};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a < b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a < b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a < b);
    }
}

TEST(HugeTest, LessEqual)
{
    {
        basic_int<byte> a = {0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x22, 0x12};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2, true);

        EXPECT_FALSE(a <= b);
    }

    {
        basic_int<byte> a = {0x21, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        basic_int<byte> a = {0x00, 0x00};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a <= b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a <= b);
    }

    {
        std::initializer_list<byte> il1 = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        std::initializer_list<byte> il2 = {0x01, 0x02, 0x03, 0x18, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        basic_int<byte> a(il1, true);
        basic_int<byte> b(il2);

        EXPECT_TRUE(a <= b);
    }
}

TEST(HugeTest, Greather)
{
    {
        basic_int<byte> a = {0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        basic_int<byte> a = {0x38, 0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a > b);
    }

    {
        basic_int<byte> a = {0x21, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        basic_int<byte> a = {0x00, 0x00};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a > b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a > b);
    }
}

TEST(HugeTest, GreatherEqual)
{
    {
        basic_int<byte> a = {0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a >= b);
    }

    {
        basic_int<byte> a = {0x38, 0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a >= b);
    }

    {
        basic_int<byte> a = {0x21, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a >= b);
    }

    {
        basic_int<byte> a = {0x00, 0x00};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a >= b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a >= b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a >= b);
    }
}

TEST(HugeTest, Equal)
{
    {
        basic_int<byte> a = {0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a == b);
    }

    {
        basic_int<byte> a = {0x38, 0x22, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        basic_int<byte> a = {0x21, 0x12};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        basic_int<byte> a = {0x00, 0x00};
        basic_int<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a == b);
    }

    {
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_int<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a == b);
    }

    std::initializer_list<byte> il = {0x01, 0x02};

    {
        basic_int<byte> a(il, true); // -258
        basic_int<byte> b(il, true); // -258

        EXPECT_TRUE(a == b);
    }

    {
        basic_int<byte> a(il, false); // 258
        basic_int<byte> b(il, true);  // -258

        EXPECT_FALSE(a == b);
    }
}

TEST(HugeTest, Inequality)
{
    auto InequalityTest = [](const basic_int<byte>& arg1, const basic_int<byte>& arg2, bool expected) {
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
        basic_int<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        bool f = (bool)a;

        EXPECT_TRUE(f);
    }

    {
        basic_int<byte> a = {0x00, 0x00, 0x00};

        bool f = (bool)a;

        EXPECT_FALSE(f);
    }
}

TEST(HugeTest, Gcd)
{
    {
        basic_int<byte> a      = {0x04, 0x2f};       // 1071
        basic_int<byte> b      = {0x01, 0xce};       // 462
        basic_int<byte> nod    = {0x15};             // 21
        basic_int<byte> actual = {0x00, 0x00, 0x00}; //

        actual = cry::gcd(a, b); // a.gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }

    {
        basic_int<byte> a      = {18};   //
        basic_int<byte> b      = {30};   //
        basic_int<byte> nod    = {6};    //
        basic_int<byte> actual = {0x00}; //

        actual = cry::gcd(a, b); // a.gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }

    {
        basic_int<byte> a      = {0x6b, 0x7d}; //
        basic_int<byte> b      = {0x31, 0x4a}; //
        basic_int<byte> nod    = {0x01};       //
        basic_int<byte> actual = {0x00, 0x00}; //

        actual = cry::gcd(a, b); // a.gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }
}

TEST(HugeTest, ModInvserse)
{
    auto ModInverseTest = [](const basic_int<byte>& arg1, const basic_int<byte>& mod, const basic_int<byte>& inv) {
        basic_int<byte> actual;

        bool exists = cry::mod_inverse(actual, arg1, mod); // arg1.mod_inverse(actual, mod);

        EXPECT_TRUE(exists);
        bool f = (actual == inv);

        EXPECT_TRUE(f);
    };

    ModInverseTest({0x1b}, {0x04, 0x09}, {0x03, 0x70});                   // a = 27, mod = 1033, inv = 880
    ModInverseTest({0x01, 0xf3}, {0x92, 0xbf, 0xb0}, {0x7a, 0x0b, 0xab}); // a = 499, mod = 9617328, inv = 7998379
    ModInverseTest({0x03}, {0x8B, 0xF9, 0xFF}, {0x5d, 0x51, 0x55});       // a = 3, mod = 9173503, inv = 6115669
    ModInverseTest({0x03}, {0x8C, 0x09, 0x9F}, {0x2e, 0xad, 0xe0});       // a = 3, mod = 9177503, inv = 3059168
}

TEST(HugeTest, DivRem)
{
    {
        basic_int<byte> expected_div(1);
        basic_int<byte> expected_rem(0);

        basic_int<byte> a = 2;
        basic_int<byte> b = 2;

        basic_int<byte> q, r;
		q = a / b; 
		r = a % b;

        EXPECT_TRUE(q == expected_div);
        EXPECT_TRUE(r == expected_rem);
    }

    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x2a, 0xaa};

        basic_int<byte> a(il1);
        basic_int<byte> b(il2);
        basic_int<byte> expected_div(il3);
        basic_int<byte> expected_rem({(byte)0x03});

        basic_int<byte> q;
        basic_int<byte> r;

		q = a / b;
		r = a % b;

        EXPECT_TRUE(q == expected_div);
        EXPECT_TRUE(r == expected_rem);
    }

    {
        basic_int<byte> a({0x10, 0x22, 0xd2, 0x74, 0x02, 0x72, 0x19, 0x2d});
        basic_int<byte> b({0x98, 0x66, 0x06, 0x3b});
        basic_int<byte> expected_div({0x1b, 0x1a, 0xfb, 0x05});
        basic_int<byte> expected_rem({0x25, 0x5a, 0x21, 0x06});

        basic_int<byte> q;
        basic_int<byte> r;

		q = a / b;
		r = a % b;

        EXPECT_TRUE(q == expected_div);
        EXPECT_TRUE(r == expected_rem);
    }
}

TEST(HugeTest, EvenOdd)
{
    auto EvenOddTest = [](const basic_int<byte>& argEven, bool flagEven, const basic_int<byte>& argOdd, bool flagOdd) {
        bool even = cry::is_even(argEven);
        EXPECT_TRUE(even == flagEven);

        bool odd = cry::is_odd(argOdd);
        EXPECT_TRUE(odd == flagOdd);
    };

    EvenOddTest(1, false, 1, true);
    EvenOddTest(2, true, 2, false);
    EvenOddTest(3, false, 3, true);
    EvenOddTest(4, true, 4, false);
}

TEST(HugeTest, PowMod)
{
    auto ModPowTest = [](const basic_int<byte>& a, const basic_int<byte>& exponent, const basic_int<byte>& modulus, const basic_int<byte>& expected) {
        basic_int<byte> actual = cry::pow_mod(a, exponent, modulus);

        bool f = (expected == actual);

        EXPECT_TRUE(f);
    };

    ModPowTest(3, 4, 11, 4);
    ModPowTest(3, 14, 497, 338);
    ModPowTest(4, 13, 497, 445);

    ModPowTest(666, 777, 2014, 666);
    ModPowTest(666, 777, 777, 666);
    ModPowTest(111111, 3, 9173503, 4051753);
    ModPowTest(4051753, 6111579, 9173503, 111111);
}

TEST(HugeTest, StringInit)
{
    auto StringInit_EXPECT_TRUE = [](const std::string& hex, const basic_int<byte>& expected) {

        basic_int<byte> x(hex);

        bool eq = (x == expected);

        EXPECT_TRUE(eq);
    };

    auto StringInit_EXPECT_ANY_THROW = [](const std::string& hex) {

        EXPECT_ANY_THROW(basic_int<byte> x(hex));

    };

    StringInit_EXPECT_TRUE("2763b4a317f", {0x02, 0x76, 0x3b, 0x4a, 0x31, 0x7f});
    StringInit_EXPECT_TRUE("  2763b4 a317f", {0x02, 0x76, 0x3b, 0x4a, 0x31, 0x7f});
    StringInit_EXPECT_TRUE("00000000000000000000000000000000002763b4a317f", {0x02, 0x76, 0x3b, 0x4a, 0x31, 0x7f});
    StringInit_EXPECT_TRUE("", {0x00});
    StringInit_EXPECT_TRUE("", {});

    StringInit_EXPECT_ANY_THROW("123aw");
    StringInit_EXPECT_ANY_THROW("*23aw");
}

TEST(HugeTest, OS2IP_IP2OS)
{
    {
        std::vector<uint8_t> octets = {1, 2, 3, 4, 5, 6, 7, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        auto ip                     = OS2IP<bigint8_t>()(octets);

        auto os = IP2OS<bigint8_t>()(ip);

        ASSERT_BYTES_EQ(octets.begin(), octets.end(), os.begin(), os.end());
    }

    {
        std::vector<uint8_t> octets = {1, 2, 3, 4, 5, 6, 7, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        auto ip                     = OS2IP<bigint16_t>()(octets);

        auto os = IP2OS<bigint16_t>()(ip);

        ASSERT_BYTES_EQ(octets.begin(), octets.end(), os.begin(), os.end());
    }

    {
        std::vector<uint8_t> octets = {1, 2, 3, 4, 5, 6, 7, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        auto ip                     = OS2IP<bigint32_t>()(octets);

        auto os = IP2OS<bigint32_t>()(ip);

        ASSERT_BYTES_EQ(octets.begin(), octets.end(), os.begin(), os.end());
    }

	{
		std::vector<uint8_t> octets = { 1, 2, 3, 4, 5, 6, 7, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
		auto ip = OS2IP<bigint64_t>()(octets);

		auto os = IP2OS<bigint64_t>()(ip);

		ASSERT_BYTES_EQ(octets.begin(), octets.end(), os.begin(), os.end());
	}
}