
#include "gtest/gtest.h"

#include <iterator>

#include "Huge.h"

using namespace std;

class HugeTest:
    public ::testing::Test
{
};


TEST(HugeTest, LogicalNOT)
{
    FAIL();
}

TEST(HugeTest, Modulus)
{
    FAIL();
}

TEST(HugeTest, ModulusAssignment)
{
    FAIL();
}

TEST(HugeTest, BitwiseAND)
{
    {
        Huge<byte> a = { 0x01, 0x02, 0x03 };
        Huge<byte> b = { 0x01, 0x02, 0x03 };

        Huge<byte> c;
        Huge<byte> ex = { 0x01, 0x02, 0x03 };

        c = (a & b);

        EXPECT_TRUE(c == ex);
    }
}

TEST(HugeTest, BitwiseANDassignment)
{
    {
        Huge<byte> a      = { 0x11, 0x22, 0x33 };
        Huge<byte> b      = { 0x00, 0x00, 0x00 };

        Huge<byte> expect = { 0x00, 0x00, 0x00 };

        a &= b;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a      = { 0x11, 0x22, 0x33 };
        Huge<byte> b      = { 0x10, 0x20, 0x30 };
        Huge<byte> c      = { 0x01, 0x02, 0x03 };

        Huge<byte> expect = { 0x00, 0x00, 0x00 };

        (b &= a) &= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, LogicalAND)
{
    FAIL();
}

TEST(HugeTest, UnaryPlus)
{
    {
        Huge<byte> a = { 0x01, 0x02, 0x03 };
        Huge<byte> b = { 0x01, 0x02, 0x03 };

        EXPECT_TRUE(a == b);
        EXPECT_TRUE(a == +b);
    }

    {
        Huge<byte> a = { 0x02, 0x02, 0x03 };
        Huge<byte> b = { 0x01, 0x02, 0x03 };

        EXPECT_FALSE(a == b);
        EXPECT_FALSE(a == +b);
    }

}

TEST(HugeTest, UnaryNegation)
{
    {
        Huge<byte> a = { 0x01, 0x02, 0x03 };
        Huge<byte> b = { 0x01, 0x02, 0x03 };

        EXPECT_TRUE(a == b);

        a = -b;

        EXPECT_TRUE(a != b);

        a = -a;

        EXPECT_TRUE(a == b);
    }
}

TEST(HugeTest, Decrement)
{
    FAIL();
}

TEST(HugeTest, LogicalOR)
{
    FAIL();
}

TEST(HugeTest, BitwiseORassignment)
{
    {
        Huge<byte> a      = { 0x11, 0x22, 0x33 };
        Huge<byte> b      = { 0x00, 0x00, 0x00 };

        Huge<byte> expect = { 0x11, 0x22, 0x33 };

        a |= b;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a      = { 0x11, 0x22, 0x33 };
        Huge<byte> b      = { 0x10, 0x20, 0x30 };
        Huge<byte> c      = { 0x04, 0x04, 0x04 };

        Huge<byte> expect = { 0x15, 0x26, 0x37 };

        (b |= a) |= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, BitwiseOR)
{
    {
        Huge<byte> a        = { 0x01, 0x02, 0x03 };
        Huge<byte> b        = { 0x11, 0x00, 0x33 };
        Huge<byte> expected = { 0x11, 0x02, 0x33 };

        Huge<byte> c        = a | b;

        EXPECT_TRUE(expected == c);
    }
}

TEST(HugeTest, ExclusiveOR)
{
    {
        Huge<byte> a        = { 0x01, 0x02, 0x03 };
        Huge<byte> b        = { 0x11, 0x00, 0x33 };
        Huge<byte> expected = { 0x10, 0x02, 0x30 };

        Huge<byte> c        = a ^ b;

        EXPECT_TRUE(expected == c);
    }
}

TEST(HugeTest, ExclusiveORassignment)
{
    {
        Huge<byte> a      = { 0x11, 0x22, 0x33 };
        Huge<byte> b      = { 0x00, 0x00, 0x00 };

        Huge<byte> expect = { 0x11, 0x22, 0x33 };

        a ^= b;

        EXPECT_TRUE(a == expect);
    }

    {
        Huge<byte> a      = { 0x11, 0x22, 0x33 };
        Huge<byte> b      = { 0x10, 0x20, 0x30 };
        Huge<byte> c      = { 0x04, 0x04, 0x04 };

        Huge<byte> expect = { 0x05, 0x06, 0x07 };

        (b ^= a) ^= c;

        EXPECT_TRUE(b == expect);
    }
}

TEST(HugeTest, BitwiseInverse)
{
    {
        Huge<byte> a = { 0x05 };
        Huge<byte> b(~a);

        Huge<byte> ex = { 0xFA };

        EXPECT_TRUE(ex == b);
    }

    {
        Huge<byte> a = { 0x05 };
        Huge<byte> b;

        b = ~a;

        Huge<byte> ex = { 0xFA };

        EXPECT_TRUE(ex == b);
    }
}

TEST(HugeTest, Addition)
{
    FAIL();
}

TEST(HugeTest, AdditionAssigment)
{
    FAIL();
}

TEST(HugeTest, Increment)
{
    FAIL();
}

TEST(HugeTest, Subtraction)
{
    FAIL();
}

TEST(HugeTest, SubtractionAssigment)
{
    FAIL();
}

TEST(HugeTest, Multiplication)
{
    FAIL();
}

TEST(HugeTest, MultiplicationAssignment)
{
    FAIL();
}

TEST(HugeTest, Division)
{
    FAIL();
}

TEST(HugeTest, DivisionAssignment)
{
    FAIL();
}

TEST(HugeTest, LeftShift)
{
    {
        Huge<byte> a = { 0x22, 0x12 };
        Huge<byte> b(a << 1);
        Huge<byte> expect = { 0x44, 0x24 };

        EXPECT_TRUE(expect == b);
    }

    {
        Huge<byte> a = { 0x32, 0x06, 0x12, 0x22, 0x12 };
        Huge<byte> c(a << 2);
        Huge<byte> expect = { 0xC8, 0x18, 0x48, 0x88, 0x48 };

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, LeftShiftAssigment)
{
    {
        Huge<byte> a = { 0x22, 0x12 };

        a <<= 1;

        Huge<byte> expect = { 0x44, 0x24 };

        EXPECT_TRUE(expect == a);
    }

    {
        Huge<byte> a = { 0x32, 0x06, 0x12, 0x22, 0x12 };
        Huge<byte> c(a << 2);
        Huge<byte> expect = { 0xC8, 0x18, 0x48, 0x88, 0x48 };

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, RightShift)
{
    {
        Huge<byte> a = { 0x22, 0x12 };
        Huge<byte> b(a >> 1);
        Huge<byte> expect = { 0x11, 0x09 };

        EXPECT_TRUE(expect == b);
    }

    {
        Huge<byte> a = { 0x32, 0x06, 0x12, 0x22, 0x12 };
        Huge<byte> c(a >> 2);
        Huge<byte> expect = { 0x0C, 0x81, 0x84, 0x88, 0x84 };

        EXPECT_TRUE(expect == c);
    }
}

TEST(HugeTest, RightShiftAssignment)
{
    {
        Huge<byte> a = { 0x22, 0x12 };

        a >>= 1;

        Huge<byte> expect = { 0x11, 0x09 };

        EXPECT_TRUE(expect == a);
    }

    {
        Huge<byte> a = { 0x32, 0x06, 0x12, 0x22, 0x12 };

        a >>= 2;

        Huge<byte> expect = { 0x0C, 0x81, 0x84, 0x88, 0x84 };

        EXPECT_TRUE(expect == a);
    }
}

TEST(HugeTest, Less)
{
    {
        Huge<byte> a = { 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a < b);
    }

    {
        Huge<byte> a = { 0x38, 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a < b);
    }

    {
        Huge<byte> a = { 0x21, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a < b);
    }

    {
        Huge<byte> a = { 0x00, 0x00 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a < b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_FALSE(a < b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_TRUE(a < b);
    }

}

TEST(HugeTest, LessEqual)
{
    {
        Huge<byte> a = { 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a <= b);
    }

    {
        Huge<byte> a = { 0x38, 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a <= b);
    }

    {
        Huge<byte> a = { 0x21, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a <= b);
    }

    {
        Huge<byte> a = { 0x00, 0x00 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a <= b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_TRUE(a <= b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_TRUE(a <= b);
    }

}

TEST(HugeTest, Greather)
{
    {
        Huge<byte> a = { 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a > b);
    }

    {
        Huge<byte> a = { 0x38, 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a > b);
    }

    {
        Huge<byte> a = { 0x21, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a > b);
    }

    {
        Huge<byte> a = { 0x00, 0x00 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a > b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_FALSE(a > b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_FALSE(a > b);
    }
}

TEST(HugeTest, GreatherEqual)
{
    {
        Huge<byte> a = { 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a >= b);
    }

    {
        Huge<byte> a = { 0x38, 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a >= b);
    }

    {
        Huge<byte> a = { 0x21, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a >= b);
    }

    {
        Huge<byte> a = { 0x00, 0x00 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a >= b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_TRUE(a >= b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_FALSE(a >= b);
    }

}

TEST(HugeTest, Equal)
{
    {
        Huge<byte> a = { 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a == b);
    }

    {
        Huge<byte> a = { 0x38, 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a == b);
    }

    {
        Huge<byte> a = { 0x21, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a == b);
    }

    {
        Huge<byte> a = { 0x00, 0x00 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a == b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_TRUE(a == b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_FALSE(a == b);
    }
}

TEST(HugeTest, Inequality)
{
    {
        Huge<byte> a = { 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_FALSE(a != b);
    }

    {
        Huge<byte> a = { 0x38, 0x22, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a != b);
    }

    {
        Huge<byte> a = { 0x21, 0x12 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a != b);
    }

    {
        Huge<byte> a = { 0x00, 0x00 };
        Huge<byte> b = { 0x22, 0x12 };

        EXPECT_TRUE(a != b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_FALSE(a != b);
    }

    {
        Huge<byte> a = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
        Huge<byte> b = { 0x01, 0x02, 0x03, 0x05, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };

        EXPECT_TRUE(a != b);
    }

}
