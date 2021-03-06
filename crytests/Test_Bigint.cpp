
#include "gtest/gtest.h"

#include "algorithm.hpp"

#include "basic_integer.hpp"
#include "utility/os2ip.hpp"

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

class Test_Bigint : public ::testing::Test
{
};

using byte = uint8_t;

TEST(Test_Bigint, LogicalNOT)
{
    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};

        EXPECT_FALSE(!a);
    }

    {
        basic_integer<byte> a = {0x00, 0x00, 0x00};

        EXPECT_TRUE(!a);
    }
}

TEST(Test_Bigint, Modulus)
{
    auto ModulusTest = [](const basic_integer<byte>& arg1, const basic_integer<byte>& arg2, const basic_integer<byte>& expected) -> void {
        auto mod = arg1 % arg2;

        EXPECT_TRUE(mod == expected);
    };

    ModulusTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
    ModulusTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
    ModulusTest({0x6f, 0x17, 0x23}, {0x17, 0x12}, {0x10, 0x83});
}

TEST(Test_Bigint, ModulusAssignment)
{
    auto ModulusAssignmentTest = [](basic_integer<byte>&& arg1, const basic_integer<byte>& arg2, const basic_integer<byte>& expected) {
        arg1 %= arg2;
        EXPECT_TRUE(arg1 == expected);
    };

    ModulusAssignmentTest({0xff, 0xff}, {0xf6, 0xf5}, {0x09, 0x0a});
}

TEST(Test_Bigint, BitwiseAND)
{
    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x01, 0x02, 0x03};

        basic_integer<byte> c;
        basic_integer<byte> ex = {0x01, 0x02, 0x03};

        c = (a & b);

        EXPECT_TRUE(c == ex);
    }
}

TEST(Test_Bigint, BitwiseANDassignment)
{
    auto Test = [](const std::initializer_list<byte>& arg1, const std::initializer_list<byte>& arg2, const std::initializer_list<byte>& arg3) -> void {
        basic_integer<byte> a(arg1);
        basic_integer<byte> b(arg2);
        basic_integer<byte> expect(arg3);

        a &= b;

        EXPECT_TRUE(a == expect);
    };

    Test({0x11, 0x22, 0x33}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00});
    Test({0x11, 0x22, 0x33}, {0x11, 0x22, 0x33}, {0x11, 0x22, 0x33});

    {
        basic_integer<byte> a = {0x11, 0x22, 0x33};
        basic_integer<byte> b = {0x10, 0x20, 0x30};
        basic_integer<byte> c = {0x01, 0x02, 0x03};

        basic_integer<byte> expect = {0x00, 0x00, 0x00};

        (b &= a) &= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(Test_Bigint, LogicalAND)
{
    auto Test = [](const std::initializer_list<byte>& arg1, const std::initializer_list<byte>& arg2, bool expect) -> void {
        basic_integer<byte> a = arg1;
        basic_integer<byte> b = arg2;

        EXPECT_TRUE((a && b) == expect);
    };

    Test({0x01, 0x02, 0x03}, {0x01, 0x02, 0x03}, true);
    Test({0x00, 0x00, 0x00}, {0x01, 0x02, 0x03}, false);
    Test({0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, false);
}

TEST(Test_Bigint, UnaryPlus)
{
    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a == b);
        EXPECT_TRUE(a == +b);
    }

    {
        basic_integer<byte> a = {0x02, 0x02, 0x03};
        basic_integer<byte> b = {0x01, 0x02, 0x03};

        EXPECT_FALSE(a == b);
        EXPECT_FALSE(a == +b);
    }
}

TEST(Test_Bigint, UnaryNegation)
{
    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a == b);

        a = -b;

        EXPECT_TRUE(a != b);

        a = -a;

        EXPECT_TRUE(a == b);
    }
}

TEST(Test_Bigint, Decrement)
{
    {
        basic_integer<byte> a      = {0x01, 0x02, 0x03};
        basic_integer<byte> expect = {0x01, 0x02, 0x02};

        a--;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_integer<byte> a      = {0x01, 0x02, 0x03};
        basic_integer<byte> expect = {0x01, 0x02, 0x02};

        --a;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_integer<byte> a = {0x45, 0x38, 0x17};
        basic_integer<byte> b;
        basic_integer<byte> expect1 = {0x45, 0x38, 0x17};
        basic_integer<byte> expect2 = {0x45, 0x38, 0x16};

        b = a--;

        EXPECT_TRUE(b == expect1);
        EXPECT_TRUE(a == expect2);
    }
}

TEST(Test_Bigint, LogicalOR)
{
    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a || b);
    }

    {
        basic_integer<byte> a = {0x00, 0x00, 0x00};
        basic_integer<byte> b = {0x01, 0x02, 0x03};

        EXPECT_TRUE(a || b);
    }

    {
        basic_integer<byte> a = {0x00, 0x00, 0x00};
        basic_integer<byte> b = {0x00, 0x00, 0x00};

        EXPECT_FALSE(a || b);
    }
}

TEST(Test_Bigint, BitwiseORassignment)
{
    {
        basic_integer<byte> a = {0x11, 0x22, 0x33};
        basic_integer<byte> b = {0x00, 0x00, 0x00};

        basic_integer<byte> expect = {0x11, 0x22, 0x33};

        a |= b;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_integer<byte> a = {0x11, 0x22, 0x33};
        basic_integer<byte> b = {0x10, 0x20, 0x30};
        basic_integer<byte> c = {0x04, 0x04, 0x04};

        basic_integer<byte> expect = {0x15, 0x26, 0x37};

        (b |= a) |= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(Test_Bigint, BitwiseOR)
{
    {
        basic_integer<byte> a        = {0x01, 0x02, 0x03};
        basic_integer<byte> b        = {0x11, 0x00, 0x33};
        basic_integer<byte> expected = {0x11, 0x02, 0x33};

        basic_integer<byte> c = a | b;

        EXPECT_TRUE(expected == c);
    }
}

TEST(Test_Bigint, ExclusiveOR)
{
    {
        basic_integer<byte> a        = {0x01, 0x02, 0x03};
        basic_integer<byte> b        = {0x11, 0x00, 0x33};
        basic_integer<byte> expected = {0x10, 0x02, 0x30};

        basic_integer<byte> c = a ^ b;

        EXPECT_TRUE(expected == c);
    }
}

TEST(Test_Bigint, ExclusiveORassignment)
{
    {
        basic_integer<byte> a = {0x11, 0x22, 0x33};
        basic_integer<byte> b = {0x00, 0x00, 0x00};

        basic_integer<byte> expect = {0x11, 0x22, 0x33};

        a ^= b;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_integer<byte> a = {0x11, 0x22, 0x33};
        basic_integer<byte> b = {0x10, 0x20, 0x30};
        basic_integer<byte> c = {0x04, 0x04, 0x04};

        basic_integer<byte> expect = {0x05, 0x06, 0x07};

        (b ^= a) ^= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(Test_Bigint, BitwiseInverse)
{
    {
        basic_integer<byte> a = {0x05};
        basic_integer<byte> b(~a);

        basic_integer<byte> ex = {0xFA};

        EXPECT_TRUE(ex == b);
    }

    {
        basic_integer<byte> a = {0x05};
        basic_integer<byte> b;

        b = ~a;

        basic_integer<byte> ex = {0xFA};

        EXPECT_TRUE(ex == b);
    }
}

TEST(Test_Bigint, Addition)
{
    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x17, 0x20, 0x11};

        basic_integer<byte> expect = {0x18, 0x22, 0x14};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x17, 0x20, 0x11};

        basic_integer<byte> expect = {0x16, 0x1e, 0x0e};

        auto c = a + (-b);

        EXPECT_TRUE(c == -expect);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x17, 0x20, 0x11};

        basic_integer<byte> expect = {0x18, 0x22, 0x14};

        auto c = -a + (-b);

        EXPECT_TRUE(c == -expect);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x17, 0x20, 0x11};

        basic_integer<byte> expect = {0x16, 0x1e, 0x0e};

        auto c = -a + (b);

        EXPECT_TRUE(c == expect);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03};
        basic_integer<byte> b = {0x00};

        basic_integer<byte> expect = {0x01, 0x02, 0x03};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }

    {
        basic_integer<byte> a = {0x00};
        basic_integer<byte> b = {0x17, 0x20, 0x11};

        basic_integer<byte> expect = {0x17, 0x20, 0x11};

        auto c = a + b;

        EXPECT_TRUE(c == expect);
    }
}

TEST(Test_Bigint, AdditionAssigment)
{
    {
        basic_integer<byte> a      = {0x01, 0x02, 0x03};
        basic_integer<byte> b      = {0x11, 0x27, 0x0a};
        basic_integer<byte> expect = {0x12, 0x29, 0x0d};

        a += b;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_integer<byte> a      = {0x01, 0x02, 0x03};
        basic_integer<byte> b      = {0x00};
        basic_integer<byte> expect = {0x01, 0x02, 0x03};

        a += b;

        EXPECT_TRUE(a == expect);
    }
}

TEST(Test_Bigint, Increment)
{
    {
        basic_integer<byte> a      = {0x01, 0x02, 0x03};
        basic_integer<byte> expect = {0x01, 0x02, 0x04};

        a++;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_integer<byte> a      = {0x01, 0x02, 0x03};
        basic_integer<byte> expect = {0x01, 0x02, 0x04};

        ++a;

        EXPECT_TRUE(a == expect);
    }

    {
        basic_integer<byte> a = {0x45, 0x38, 0x17};
        basic_integer<byte> b;
        basic_integer<byte> expect1 = {0x45, 0x38, 0x17};
        basic_integer<byte> expect2 = {0x45, 0x38, 0x18};

        b = a++;

        EXPECT_TRUE(b == expect1);
        EXPECT_TRUE(a == expect2);
    }
}

TEST(Test_Bigint, Subtraction)
{
    { // (a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x00};
        std::initializer_list<byte> il3 = {0x38, 0x22, 0x12};

        basic_integer<byte> a(il1, true);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x22, 0x12};
        std::initializer_list<byte> il3 = {0x38, 0x44, 0x24};

        basic_integer<byte> a(il1, true);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(-b), |a|>|b|
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x17, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x4f, 0x33, 0xab};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2, true);

        basic_integer<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(b), |a|<|b|
        std::initializer_list<byte> il1 = {0x22, 0x12};
        std::initializer_list<byte> il2 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x38, 0x44, 0x24};

        basic_integer<byte> a(il1, true);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(-b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x4f, 0x33, 0xab};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2, true);

        basic_integer<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    // =========================================================

    { // (-a)-(-b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        basic_integer<byte> a(il1, true);
        basic_integer<byte> b(il2, true);

        basic_integer<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(b), |a|<|b|
        std::initializer_list<byte> il1 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);

        basic_integer<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (-a)-(-b), |a|>|b|
        std::initializer_list<byte> il1 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        basic_integer<byte> a(il1, true);
        basic_integer<byte> b(il2, true);

        basic_integer<byte> expected(il3, true);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }

    { // (a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x28, 0x11, 0x99};
        std::initializer_list<byte> il2 = {0x17, 0x22, 0x12};
        std::initializer_list<byte> il3 = {0x10, 0xef, 0x87};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);

        basic_integer<byte> expected(il3);

        auto c = a - b;

        EXPECT_TRUE(c == expected);
    }
}

TEST(Test_Bigint, SubtractionAssigment)
{
    { // (a)-(b), |a|>|b|
        std::initializer_list<byte> il1 = {0x0a};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x04};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);

        basic_integer<byte> expected(il3);

        a -= b;

        EXPECT_TRUE(a == expected);
    }
}

TEST(Test_Bigint, Multiplication)
{
    {
        std::initializer_list<byte> il1 = {0x0a};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x3c};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected(il3);

        basic_integer<byte> c = a * b;
        EXPECT_TRUE(c == expected);
    }

    {
        std::initializer_list<byte> il1 = {0xfa, 0x0a};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x05, 0xdc, 0x3c};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected(il3);

        basic_integer<byte> c = a * b;
        EXPECT_TRUE(c == expected);
    }
}

TEST(Test_Bigint, MultiplicationAssignment)
{
    std::initializer_list<byte> il1 = {0xfa, 0x0a};
    std::initializer_list<byte> il2 = {0x06};
    std::initializer_list<byte> il3 = {0x05, 0xdc, 0x3c};

    basic_integer<byte> a(il1);
    basic_integer<byte> b(il2);
    basic_integer<byte> expected(il3);

    a *= b;

    EXPECT_TRUE(a == expected);
}

TEST(Test_Bigint, Division)
{
    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x2a, 0xaa};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0xf6, 0xf5};
        std::initializer_list<byte> il3 = {0x01};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0xff, 0xf1, 0xff, 0xfa, 0x56, 0x28, 0xff};
        std::initializer_list<byte> il2 = {0xf6, 0xf5, 0x03, 17};
        std::initializer_list<byte> il3 = {0x01, 0x09, 0x51, 0x3e};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected_div(il3);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }

    {
        std::initializer_list<byte> il1 = {0x6f, 0x17, 0x23};
        std::initializer_list<byte> il2 = {0x17, 0x12};
        std::initializer_list<byte> il3 = {0x04, 0xd0};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2, true);
        basic_integer<byte> expected_div(il3, true);

        auto c = a / b;

        EXPECT_TRUE(c == expected_div);
    }
}

TEST(Test_Bigint, DivisionAssignment)
{
    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x2a, 0xaa};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected_div(il3);

        a /= b;

        EXPECT_TRUE(a == expected_div);
    }
}

TEST(Test_Bigint, LeftShift)
{
    {
        basic_integer<byte> a = {0x22, 0x12};
        basic_integer<byte> b(a << 1);
        basic_integer<byte> expect = {0x44, 0x24};

        EXPECT_TRUE(expect == b);
    }

    {
        basic_integer<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        basic_integer<byte> c(a << 2);
        basic_integer<byte> expect = {0xC8, 0x18, 0x48, 0x88, 0x48};

        EXPECT_TRUE(expect == c);
    }
}

TEST(Test_Bigint, LeftShiftAssigment)
{
    {
        basic_integer<byte> a = {0x22, 0x12};

        a <<= 1;

        basic_integer<byte> expect = {0x44, 0x24};

        EXPECT_TRUE(expect == a);
    }

    {
        basic_integer<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        basic_integer<byte> c(a << 2);
        basic_integer<byte> expect = {0xC8, 0x18, 0x48, 0x88, 0x48};

        EXPECT_TRUE(expect == c);
    }
}

TEST(Test_Bigint, RightShift)
{
    {
        basic_integer<byte> a = {0x22, 0x12};
        basic_integer<byte> b(a >> 1);
        basic_integer<byte> expect = {0x11, 0x09};

        EXPECT_TRUE(expect == b);
    }

    {
        basic_integer<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};
        basic_integer<byte> c(a >> 2);
        basic_integer<byte> expect = {0x0C, 0x81, 0x84, 0x88, 0x84};

        EXPECT_TRUE(expect == c);
    }
}

TEST(Test_Bigint, RightShiftAssignment)
{
    {
        basic_integer<byte> a = {0x22, 0x12};

        a >>= 1;

        basic_integer<byte> expect = {0x11, 0x09};

        EXPECT_TRUE(expect == a);
    }

    {
        basic_integer<byte> a = {0x32, 0x06, 0x12, 0x22, 0x12};

        a >>= 2;

        basic_integer<byte> expect = {0x0C, 0x81, 0x84, 0x88, 0x84};

        EXPECT_TRUE(expect == a);
    }
}

TEST(Test_Bigint, Less)
{
    {
        basic_integer<byte> a = {0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a < b);
    }

    {
        basic_integer<byte> a = {0x38, 0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a < b);
    }

    {
        basic_integer<byte> a = {0x21, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a < b);
    }

    {
        basic_integer<byte> a = {0x00, 0x00};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a < b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a < b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a < b);
    }
}

TEST(Test_Bigint, LessEqual)
{
    {
        basic_integer<byte> a = {0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        std::initializer_list<byte> il1 = {0x38, 0x22, 0x12};
        std::initializer_list<byte> il2 = {0x22, 0x12};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2, true);

        EXPECT_FALSE(a <= b);
    }

    {
        basic_integer<byte> a = {0x21, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        basic_integer<byte> a = {0x00, 0x00};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a <= b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a <= b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a <= b);
    }

    {
        std::initializer_list<byte> il1 = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        std::initializer_list<byte> il2 = {0x01, 0x02, 0x03, 0x18, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        basic_integer<byte> a(il1, true);
        basic_integer<byte> b(il2);

        EXPECT_TRUE(a <= b);
    }
}

TEST(Test_Bigint, Greather)
{
    {
        basic_integer<byte> a = {0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        basic_integer<byte> a = {0x38, 0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a > b);
    }

    {
        basic_integer<byte> a = {0x21, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        basic_integer<byte> a = {0x00, 0x00};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a > b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a > b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a > b);
    }
}

TEST(Test_Bigint, GreatherEqual)
{
    {
        basic_integer<byte> a = {0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a >= b);
    }

    {
        basic_integer<byte> a = {0x38, 0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a >= b);
    }

    {
        basic_integer<byte> a = {0x21, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a >= b);
    }

    {
        basic_integer<byte> a = {0x00, 0x00};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a >= b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a >= b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a >= b);
    }
}

TEST(Test_Bigint, Equal)
{
    {
        basic_integer<byte> a = {0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_TRUE(a == b);
    }

    {
        basic_integer<byte> a = {0x38, 0x22, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        basic_integer<byte> a = {0x21, 0x12};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        basic_integer<byte> a = {0x00, 0x00};
        basic_integer<byte> b = {0x22, 0x12};

        EXPECT_FALSE(a == b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_TRUE(a == b);
    }

    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
        basic_integer<byte> b = {0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        EXPECT_FALSE(a == b);
    }

    std::initializer_list<byte> il = {0x01, 0x02};

    {
        basic_integer<byte> a(il, true); // -258
        basic_integer<byte> b(il, true); // -258

        EXPECT_TRUE(a == b);
    }

    {
        basic_integer<byte> a(il, false); // 258
        basic_integer<byte> b(il, true);  // -258

        EXPECT_FALSE(a == b);
    }
}

TEST(Test_Bigint, Inequality)
{
    auto InequalityTest = [](const basic_integer<byte>& arg1, const basic_integer<byte>& arg2, bool expected) {
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

TEST(Test_Bigint, BOOL)
{
    {
        basic_integer<byte> a = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

        bool f = (bool)a;

        EXPECT_TRUE(f);
    }

    {
        basic_integer<byte> a = {0x00, 0x00, 0x00};

        bool f = (bool)a;

        EXPECT_FALSE(f);
    }
}

TEST(Test_Bigint, Gcd)
{
    {
        basic_integer<byte> a      = {0x04, 0x2f};       // 1071
        basic_integer<byte> b      = {0x01, 0xce};       // 462
        basic_integer<byte> nod    = {0x15};             // 21
        basic_integer<byte> actual = {0x00, 0x00, 0x00}; //

        actual = cry::gcd(a, b); // a.gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }

    {
        basic_integer<byte> a      = {18};   //
        basic_integer<byte> b      = {30};   //
        basic_integer<byte> nod    = {6};    //
        basic_integer<byte> actual = {0x00}; //

        actual = cry::gcd(a, b); // a.gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }

    {
        basic_integer<byte> a      = {0x6b, 0x7d}; //
        basic_integer<byte> b      = {0x31, 0x4a}; //
        basic_integer<byte> nod    = {0x01};       //
        basic_integer<byte> actual = {0x00, 0x00}; //

        actual = cry::gcd(a, b); // a.gcd(b);

        bool f = (actual == nod);

        EXPECT_TRUE(f);
    }
}

TEST(Test_Bigint, ModInvserse)
{
    auto ModInverseTest = [](const basic_integer<byte>& arg1, const basic_integer<byte>& mod, const basic_integer<byte>& inv) {
        basic_integer<byte> actual;

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

TEST(Test_Bigint, DivRem)
{
    {
        basic_integer<byte> expected_div(1);
        basic_integer<byte> expected_rem(0);

        basic_integer<byte> a = 2;
        basic_integer<byte> b = 2;

        basic_integer<byte> q, r;
		q = a / b; 
		r = a % b;

        EXPECT_TRUE(q == expected_div);
        EXPECT_TRUE(r == expected_rem);
    }

    {
        std::initializer_list<byte> il1 = {0xff, 0xff};
        std::initializer_list<byte> il2 = {0x06};
        std::initializer_list<byte> il3 = {0x2a, 0xaa};

        basic_integer<byte> a(il1);
        basic_integer<byte> b(il2);
        basic_integer<byte> expected_div(il3);
        basic_integer<byte> expected_rem({(byte)0x03});

        basic_integer<byte> q;
        basic_integer<byte> r;

		q = a / b;
		r = a % b;

        EXPECT_TRUE(q == expected_div);
        EXPECT_TRUE(r == expected_rem);
    }

    {
        basic_integer<byte> a({0x10, 0x22, 0xd2, 0x74, 0x02, 0x72, 0x19, 0x2d});
        basic_integer<byte> b({0x98, 0x66, 0x06, 0x3b});
        basic_integer<byte> expected_div({0x1b, 0x1a, 0xfb, 0x05});
        basic_integer<byte> expected_rem({0x25, 0x5a, 0x21, 0x06});

        basic_integer<byte> q;
        basic_integer<byte> r;

		q = a / b;
		r = a % b;

        EXPECT_TRUE(q == expected_div);
        EXPECT_TRUE(r == expected_rem);
    }
}

TEST(Test_Bigint, EvenOdd)
{
    auto EvenOddTest = [](const basic_integer<byte>& argEven, bool flagEven, const basic_integer<byte>& argOdd, bool flagOdd) {
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

TEST(Test_Bigint, PowMod)
{
    auto ModPowTest = [](const basic_integer<byte>& a, const basic_integer<byte>& exponent, const basic_integer<byte>& modulus, const basic_integer<byte>& expected) {
        basic_integer<byte> actual = cry::pow_mod(a, exponent, modulus);

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

TEST(Test_Bigint, StringInit)
{
    auto StringInit_EXPECT_TRUE = [](const std::string& hex, const basic_integer<byte>& expected) {

        basic_integer<byte> x(hex);

        bool eq = (x == expected);

        EXPECT_TRUE(eq);
    };

    auto StringInit_EXPECT_ANY_THROW = [](const std::string& hex) {

        EXPECT_ANY_THROW(basic_integer<byte> x(hex));

    };

    StringInit_EXPECT_TRUE("2763b4a317f", {0x02, 0x76, 0x3b, 0x4a, 0x31, 0x7f});
    StringInit_EXPECT_TRUE("  2763b4 a317f", {0x02, 0x76, 0x3b, 0x4a, 0x31, 0x7f});
    StringInit_EXPECT_TRUE("00000000000000000000000000000000002763b4a317f", {0x02, 0x76, 0x3b, 0x4a, 0x31, 0x7f});
    StringInit_EXPECT_TRUE("", {0x00});
    StringInit_EXPECT_TRUE("", {});

    StringInit_EXPECT_ANY_THROW("123aw");
    StringInit_EXPECT_ANY_THROW("*23aw");
}

TEST(Test_Bigint, OS2IP_IP2OS)
{
    {
        std::vector<uint8_t> octets = {1, 2, 3, 4, 5, 6, 7, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        auto ip                     = OS2IP<bigint_t>()(octets);

        auto os = I2OSP<bigint_t>()(ip);

        ASSERT_BYTES_EQ(octets.begin(), octets.end(), os.begin(), os.end());
    }

    /*{
        std::vector<uint8_t> octets = {1, 2, 3, 4, 5, 6, 7, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        auto ip                     = OS2IP<bigint16_t>()(octets);

        auto os = I2OSP<bigint16_t>()(ip);

        ASSERT_BYTES_EQ(octets.begin(), octets.end(), os.begin(), os.end());
    }

    {
        std::vector<uint8_t> octets = {1, 2, 3, 4, 5, 6, 7, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        auto ip                     = OS2IP<bigint32_t>()(octets);

        auto os = I2OSP<bigint32_t>()(ip);

        ASSERT_BYTES_EQ(octets.begin(), octets.end(), os.begin(), os.end());
    }

	{
		std::vector<uint8_t> octets = { 1, 2, 3, 4, 5, 6, 7, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
		auto ip = OS2IP<bigint64_t>()(octets);

		auto os = I2OSP<bigint64_t>()(ip);

		ASSERT_BYTES_EQ(octets.begin(), octets.end(), os.begin(), os.end());
	}*/
}