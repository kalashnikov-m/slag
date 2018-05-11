/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include "algorithm.hpp"
#include "googletest/include/gtest/gtest.h"
#include <thread>
#include "rsa/rsa.hpp"
#include "rsa/rsaes_oaep.hpp"

namespace
{

}

int main(int argc, char** argv)
{
    /*cry::bigint_t modulus;
    cry::bigint_t d;
    cry::bigint_t e = 65537;

    cry::rsa::generate_key_pair_mt(modulus, d, 65537, 1024);

    std::cout << modulus << "\n";
    std::cout << d << "\n";

    std::vector<uint8_t> plain = {'a', 'b', 'c'};

	std::vector<uint8_t> dst(128);
	cry::rsa::rsaes_oaep<>::encrypt(plain.begin(), plain.end(), dst.begin(), e, modulus, 1024);

	std::vector<uint8_t> p;
	cry::rsa::rsaes_oaep<>::decrypt(dst.begin(), dst.end(), std::back_inserter(p), d, modulus, 1024);

    /*auto plainText = cry::OS2IP<cry::bigint_t>()(plain.begin(), plain.end());

    auto signature = cry::pow_mod(plainText, d, modulus);

    auto dst = cry::pow_mod(signature, e, modulus);*/

    ::testing::InitGoogleTest(&argc, argv);

    int rval = RUN_ALL_TESTS();

    return rval;
}
