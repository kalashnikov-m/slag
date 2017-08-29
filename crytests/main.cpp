/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include <algorithm.hpp>

#include <cctype>

#include "basic_int.hpp"
#include "googletest/include/gtest/gtest.h"
#include "emsa_pkcs1.hpp"
#include "rsa/rsassa_pkcs1.hpp"

using namespace std;
using namespace cry;

int main(int argc, char **argv) 
{

  ::testing::InitGoogleTest(&argc, argv);

  int rval = RUN_ALL_TESTS();

  return rval;
}
