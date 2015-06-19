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
    byte a[]        =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x03
    };
    byte b[]        =
    {
        0x00, 0x00, 0x00, 0x11
    };
    byte expected[] = { 0x1a, 0x14 };
    byte actual[8]  = { 0x00 };

    uadd(end(actual), begin(a), end(a), begin(b), end(b));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a2[]        = { 0x00, 0xff, 0xff };
    byte b2[]        =
    {
        0x00, 0x00, 0xff, 0xff
    };
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
    byte a[]        =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x03
    };
    byte b[]        =
    {
        0x00, 0x00, 0x00, 0x11
    };
    byte expected[] = { 0x01, 0xBA, 0x33 };
    byte actual[8]  = { 0x00 };

    umul(begin(actual), end(actual), begin(a), end(a), begin(b), end(b));

    bool eq = ASSERT_BYTES_EQ(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));

    EXPECT_TRUE(eq);

    // ==============================================================
    byte a2[]        = { 0x00, 0xff, 0xff };
    byte b2[]        =
    {
        0x00, 0x00, 0xff, 0xff
    };
    byte expected2[] =
    {
        0xFF, 0xFE, 0x00, 0x01
    };

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

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

    /*
     * byte a[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x03};
     * byte b[] = {0x00, 0x00, 0x00, 0x11};
     * byte c[8] = {0x00};
     * byte d[8] = {0x00};
     *
     * { // ===== UDIV =====
     *   byte* result = udiv(std::begin(c), std::end(c),
     *   std::begin(d), std::end(d),
     *   std::begin(a), std::end(a),
     *   std::begin(b), std::end(b));
     *
     * /*for_each(begin(c), end(c), [](byte b)
     * {
     *   printf("%02x.", b);
     * });
     *
     * printf("\n");
     * }
     */

    /*
     * { // ===== UMUL =====
     *   byte a[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10};
     *   byte b[] = {0x00, 0x00, 0x00, 0xc0};
     *   byte c[8] = {0x00};
     *
     *   byte* result = umul(begin(c), end(c),
     *       begin(a), end(a),
     *       begin(b), end(b));
     *
     *   for(int i = 0; i < sizeof(c); ++i)
     *   {
     *       printf("%02x.", c[i]);
     *   }
     *
     *   printf("\n");
     * }
     */

    // cout << get_high_bit(begin(a), end(a));
    // cout << ucmp(begin(a), end(a), begin(b), end(b));

    /*
     * auto result = usub(end(c), begin(a), end(a), begin(b), end(b));
     * for_each(result, end(c), [](byte bt)
     * {
     *   printf("%02x.", bt);
     * });
     *
     * printf("\n");
     */

    /*
     * rotate_right_n(begin(a), end(a), 1);
     * rotate_right(begin(a), end(a));
     * for_each(begin(a), end(a), [](byte bt)
     * {
     *   printf("%02x.", bt);
     * });
     * printf("\n");
     */

    /*
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     *
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     *
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     * shift_right(begin(a), end(a));
     */

    /*
     * shift_right_n(begin(a), end(a), 33);
     * for_each(begin(a), end(a), [](byte bt)
     * {
     *   printf("%02x.", bt);
     * });
     * printf("\n");
     */

    /*
     * byte* result = usub(c + lc, a, a + la, b, b + lb);
     * for_each(begin(c), end(c), [](byte b)
     * {
     *   printf("%02x.", b);
     * });
     */

    // printf("\n");

    /*
     * byte* result = uadd(end(c),
     *       begin(a), end(a),
     *       begin(b), end(b));
     *
     * for_each(result, end(c), [](byte b)
     * {
     *   printf("%02x:", b);
     * });
     */

    // printf("\n");
    return 0;
}


//~ Formatted by Jindent --- http://www.jindent.com
