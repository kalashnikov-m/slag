
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

        // EXPECT_TRUE(a == +b);
    }
}

TEST(HugeTest, BitwiseANDassignment)
{
    FAIL();
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
    FAIL();
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
    FAIL();
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
    FAIL();
}

TEST(HugeTest, LeftShiftAssigment)
{
    FAIL();
}

TEST(HugeTest, RightShift)
{
    FAIL();
}

TEST(HugeTest, RightShiftAssignment)
{
    FAIL();
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
