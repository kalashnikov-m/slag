
#include "gtest/gtest.h"

#include <iterator>

#include "HugeCore.h"
#include "Huge.h"

using namespace std;

TEST(HugePlus, Plus)
{
    Huge<byte> a = { 0x02, 0x12 };
    Huge<byte> b = { 0x20, 0x12 };
    Huge<byte> c;
    Huge<byte> e = { 0x22, 0x24 };

    c          = a + b;

    auto equal = (c == e);

    EXPECT_TRUE(equal);
}

TEST(HugeSubtract, Subtract)
{
    Huge<byte> a = { 0x22, 0x12 };
    Huge<byte> b = { 0x20, 0x12 };
    Huge<byte> c;
    Huge<byte> e = { 0x22, 0x24 };

    c          = a - b;

    auto equal = (c == e);

    EXPECT_TRUE(equal);
}

TEST(HugeMultiply, Multiply)
{
    Huge<byte> a = { 0x22, 0x12 };
    Huge<byte> b = { 0x20, 0x12 };
    Huge<byte> c;
    Huge<byte> e = { 0x22, 0x24 };

    c          = a * b;

    auto equal = (c == e);

    EXPECT_TRUE(equal);
}

TEST(HugeDivRem, DivRem)
{
    Huge<byte> a = { 0x22, 0x12 };
    Huge<byte> b = { 0x20, 0x12 };
    Huge<byte> div;
    Huge<byte> rem;

    div = a / b;
    rem = a % b;
}

TEST(HugeShiftLeft, ShiftLeft)
{
    Huge<int> a;

    a << 3;
}

TEST(HugeShiftRigth, ShiftRight)
{
    Huge<int> a;

    a >> 7;
}

TEST(HugeLess, Less)
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

TEST(HugeLessEqual, LessEqual)
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

TEST(HugeGreather, Greather)
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

TEST(HugeGreatherEqual, GreatherEqual)
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

TEST(HugeEqual, Equal)
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

TEST(HugeNotEqual, NotEqual)
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
