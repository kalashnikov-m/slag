/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include "googletest/include/gtest/gtest.h"

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);

  int rval = RUN_ALL_TESTS();

  return rval;
}
