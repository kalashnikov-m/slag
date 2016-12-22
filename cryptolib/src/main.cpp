/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include "gtest/gtest.h"

#include <algorithm.hpp>

using namespace std;

int main(int argc, char** argv)
{
    std::cout << std::boolalpha;

    bool prime = cry::IsProbablyPrime(17, 3);
    cout << prime << "\n";

    prime = cry::IsProbablyPrime(21, 3);
    cout << prime << "\n";

    prime = cry::IsProbablyPrime(111, 3);
    cout << prime << "\n";

    prime = cry::IsProbablyPrime(3361, 3);
    cout << prime << "\n";

    return 0;
    //::testing::InitGoogleTest(&argc, argv);

    // return RUN_ALL_TESTS();
}
