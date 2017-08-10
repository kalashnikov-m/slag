/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include <algorithm.hpp>

#include <cctype>

#include "googletest/include/gtest/gtest.h"

using namespace std;
using namespace cry;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

    return 0;
}
