/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "huge.h"
#include "gtest/gtest.h"

using namespace std;

static void dump(byte* b, byte* e)
{
    while (b != e)
    {
        printf("%02x:", *b);

        ++b;
    }

    printf("\n");
}

/*
 *
 */
static bool ASSERT_BYTES_EQ(byte* f1, byte* l1, byte* f2, byte* l2)
{
    bool ret = false;

    while ((f1 != l1) && (*f1 == 0x00))
    {
        ++f1;
    }

    while ((f2 != l2) && (*f2 == 0x00))
    {
        ++f2;
    }

    while ((f1 != l1) && (f2 != l2))
    {
        if (*f1 != *f2)
        {
            return false;
        }

        ++f1;
        ++f2;
    }

    return true;
}

TEST(HUGE_TEST, uadd_test)
{
    byte a[]        = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x03 };
    byte b[]        = { 0x00, 0x00, 0x00, 0x11 };
    byte expected[] = { 0x1a, 0x14 };
    byte actual[8]  = { 0x00 };

    uadd(end(actual), begin(a), end(a), begin(b), end(b));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a2[]        = { 0x00, 0xff, 0xff };
    byte b2[]        = { 0x00, 0x00, 0xff, 0xff };
    byte expected2[] = { 0x01, 0xFF, 0xFE };

    std::fill(std::begin(actual), std::end(actual), 0x00);
    uadd(end(actual), begin(a2), end(a2), begin(b2), end(b2));

    eq = ASSERT_BYTES_EQ(std::begin(expected2), std::end(expected2), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a3[]        = { 0x00, 0xff };
    byte b3[]        = { 0x00 };
    byte expected3[] = { 0xFF };

    std::fill(std::begin(actual), std::end(actual), 0x00);
    uadd(end(actual), begin(a3), end(a3), begin(b3), end(b3));

    eq = ASSERT_BYTES_EQ(std::begin(expected3), std::end(expected3), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);
}

TEST(HUGE_TEST, umul_test)
{
    byte a[]        = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x03 };
    byte b[]        = { 0x00, 0x00, 0x00, 0x11 };
    byte expected[] = { 0x01, 0xBA, 0x33 };
    byte actual[8]  = { 0x00 };

    umul(begin(actual), end(actual), begin(a), end(a), begin(b), end(b));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a2[]        = { 0x00, 0xff, 0xff };
    byte b2[]        = { 0x00, 0x00, 0xff, 0xff };
    byte expected2[] = { 0xFF, 0xFE, 0x00, 0x01 };

    std::fill(std::begin(actual), std::end(actual), 0x00);
    umul(begin(actual), end(actual), begin(a2), end(a2), begin(b2), end(b2));

    eq = ASSERT_BYTES_EQ(std::begin(expected2), std::end(expected2), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a3[]        = { 0x00, 0xff };
    byte b3[]        = { 0x00 };
    byte expected3[] = { 0x00 };

    std::fill(std::begin(actual), std::end(actual), 0x00);
    umul(begin(actual), end(actual), begin(a3), end(a3), begin(b3), end(b3));

    eq = ASSERT_BYTES_EQ(std::begin(expected3), std::end(expected3), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);
}

TEST(HUGE_TEST, usub_test)
{
    byte a[]        = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01 };
    byte b[]        = { 0x00, 0x00, 0x00, 0x80 };
    byte expected[] = { 0x00, 0x81 };
    byte actual[8]  = { 0x00 };

    auto ptr = usub(end(actual), begin(a), end(a), begin(b), end(b));
    dump(ptr, end(actual));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), /* std::begin(actual) */ ptr, std::end(actual));

    EXPECT_TRUE(eq);

    byte a1[]        = { 0x01, 0x01, };
    byte b1[]        = { 0x02, };

    byte expected1[] = { 0xff };
    byte actual1[4]  = { 0x00 };

    ptr = usub(end(actual1), begin(a1), end(a1), begin(b1), end(b1));

    eq  = ASSERT_BYTES_EQ(std::begin(expected1), std::end(expected1), /* std::begin(actual) */ ptr, std::end(actual1));

    byte a2[]        = { 0x00, 0x01, 0x00, };
    byte b2[]        = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00 };

    byte expected2[] = { 0x00 };
    byte actual2[4]  = { 0x00 };

    ptr = usub(end(actual2), begin(a2), end(a2), begin(b2), end(b2));

    eq  = ASSERT_BYTES_EQ(std::begin(expected2), std::end(expected2), std::begin(actual), std::end(actual2));
}

TEST(HUGE_TEST, udiv_test)
{
    byte a[]            = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x07 };
    byte b[]            = { 0x00, 0x00, 0x02 };

    byte expected_div[] = { 0x00, 0x80, 0x83 };
    byte expected_rem[] = { 0x00, 0x00, 0x01 };
    
    byte actual_div[8]  = { 0x00 };
    byte actual_rem[8]  = { 0x00 };

    auto it = udiv(begin(actual_div), end(actual_div), begin(actual_rem), end(actual_rem), begin(a), end(a), begin(b), end(b));

    dump(it, end(actual_div));
    //dump(begin(actual_rem), end(actual_rem));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected_div), std::end(expected_div), std::begin(actual_div), std::end(actual_div));
    EXPECT_TRUE(eq);
    
    eq = ASSERT_BYTES_EQ(std::begin(expected_rem), std::end(expected_rem), std::begin(actual_rem), std::end(actual_rem));
    EXPECT_TRUE(eq);
    
    // ==============================================================

    byte a1[]            = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x07 };
    byte b1[]            = { 0x00, 0x00, 0x03 };

    byte expected_div1[] = { 0x00, 0x55, 0xad };
    byte expected_rem1[] = { 0x00, 0x00, 0x00 };
    
    byte actual_div1[8]  = { 0x00 };
    byte actual_rem1[8]  = { 0x00 };

    it = udiv(begin(actual_div1), end(actual_div1), begin(actual_rem1), end(actual_rem1), begin(a1), end(a1), begin(b1), end(b1));

    dump(it, end(actual_div1));
    //dump(begin(actual_rem), end(actual_rem));

    eq = ASSERT_BYTES_EQ(std::begin(expected_div1), std::end(expected_div1), std::begin(actual_div1), std::end(actual_div1));
    EXPECT_TRUE(eq);
    
    eq = ASSERT_BYTES_EQ(std::begin(expected_rem1), std::end(expected_rem1), std::begin(actual_rem1), std::end(actual_rem1));
    EXPECT_TRUE(eq);
    
    // ==============================================================

    byte a2[]            = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x07 };
    byte b2[]            = { 0x00, 0x03, 0x03 };

    byte expected_div2[] = { 0x00, 0x55 };
    byte expected_rem2[] = { 0x00, 0x01, 0x08 };
    
    byte actual_div2[8]  = { 0x00 };
    byte actual_rem2[8]  = { 0x00 };

    it = udiv(begin(actual_div2), end(actual_div2), begin(actual_rem2), end(actual_rem2), begin(a2), end(a2), begin(b2), end(b2));

    dump(it, end(actual_div2));
    //dump(begin(actual_rem), end(actual_rem));

    eq = ASSERT_BYTES_EQ(std::begin(expected_div2), std::end(expected_div2), std::begin(actual_div2), std::end(actual_div2));
    EXPECT_TRUE(eq);
    
    eq = ASSERT_BYTES_EQ(std::begin(expected_rem2), std::end(expected_rem2), std::begin(actual_rem2), std::end(actual_rem2));
    EXPECT_TRUE(eq);
    
    // ==============================================================

    byte a3[]            = { 0x00, 0x00, 0x16, 0x14, 0x55, 0xD6, 0x42, 0x21, 0x22, 0x41};
    byte b3[]            = { 0x00, 0x43, 0x15, 0x55, 0x22, 0x24, 0x41, 0x4F };

    byte expected_div3[] = { 0x00, 0x54 };
    byte expected_rem3[] = { 0x00, 0x11, 0x55, 0xE7, 0x0E, 0x3B, 0xB4, 0x55 };
    
    byte actual_div3[8]  = { 0x00 };
    byte actual_rem3[8]  = { 0x00 };

    it = udiv(begin(actual_div3), end(actual_div3), begin(actual_rem3), end(actual_rem3), begin(a3), end(a3), begin(b3), end(b3));

    dump(it, end(actual_div3));
    //dump(begin(actual_rem3), end(actual_rem3));

    eq = ASSERT_BYTES_EQ(std::begin(expected_div3), std::end(expected_div3), std::begin(actual_div3), std::end(actual_div3));
    EXPECT_TRUE(eq);
    
    eq = ASSERT_BYTES_EQ(std::begin(expected_rem3), std::end(expected_rem3), std::begin(actual_rem3), std::end(actual_rem3));
    EXPECT_TRUE(eq);
    
    // ==============================================================

    byte a4[]            = { 0x00, 0x00, 0x16, 0x14, 0x55, 0xD6, 0x42, 0x21, 0x22, 0x41};
    byte b4[]            = { 0x00, 0x24, 0x41, 0x4F };

    byte expected_div4[] = { 0x00, 0x9B, 0xE7, 0x8D, 0xBE, 0xAA };
    byte expected_rem4[] = { 0x12, 0x21, 0xCB };
    
    byte actual_div4[8]  = { 0x00 };
    byte actual_rem4[8]  = { 0x00 };

    it = udiv(begin(actual_div4), end(actual_div4), begin(actual_rem4), end(actual_rem4), begin(a4), end(a4), begin(b4), end(b4));

    dump(it, end(actual_div4));
    //dump(begin(actual_rem3), end(actual_rem3));

    eq = ASSERT_BYTES_EQ(std::begin(expected_div4), std::end(expected_div4), std::begin(actual_div4), std::end(actual_div4));
    EXPECT_TRUE(eq);
    
    eq = ASSERT_BYTES_EQ(std::begin(expected_rem4), std::end(expected_rem4), std::begin(actual_rem4), std::end(actual_rem4));
    EXPECT_TRUE(eq);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

    return 0;
}
