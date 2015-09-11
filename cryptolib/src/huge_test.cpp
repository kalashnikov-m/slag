
#include "gtest/gtest.h"

#include <iterator>

#include "hugecore.h"
#include "huge.h"

using namespace std;

TEST(HugePlus, Plus)
{
    Huge<int> a;
    Huge<int> b;
    Huge<int> c;

    c = a + b;
}

TEST(HugeSubtract, Subtract)
{
    Huge<int> a;
    Huge<int> b;
    Huge<int> c;

    c = a - b;
}

TEST(HugeMultiply, Multiply)
{
    Huge<int> a;
    Huge<int> b;
    Huge<int> c;

    c = a * b;
}

TEST(HugeDivRem, DivRem)
{
    Huge<int> a;
    Huge<int> b;
    Huge<int> div;
    Huge<int> rem;

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
    Huge<int> a;
    Huge<int> b;

    a < b;
}

TEST(HugeGreather, Greather)
{
    Huge<int> a;
    Huge<int> b;

    a > b;
}

TEST(HugeEqual, Equal)
{
    Huge<int> a;
    Huge<int> b;

    a == b;
}


//~ Formatted by Jindent --- http://www.jindent.com
