
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
    Huge<byte> a = { 0x22, 0x12 };
    Huge<byte> b = { 0x20, 0x12 };

    EXPECT_TRUE(a < b);
}

TEST(HugeGreather, Greather)
{
    Huge<byte> a = { 0x22, 0x12 };
    Huge<byte> b = { 0x20, 0x12 };

    EXPECT_TRUE(a > b);
}

TEST(HugeEqual, Equal)
{
    Huge<byte> a = { 0x22, 0x12 };
    Huge<byte> b = { 0x20, 0x12 };
    
    EXPECT_TRUE(a == b);
}
