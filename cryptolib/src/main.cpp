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

// class Fake : public ::testing::Test {};

TEST(Fake, fake)
{
    {
        byte a[] = { 0x01 };

        HUGE_Not(begin(a), end(a));
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}


//~ Formatted by Jindent --- http://www.jindent.com
