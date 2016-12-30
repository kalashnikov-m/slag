/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include "gtest/gtest.h"

#include <algorithm.hpp>
#include <sha1.hpp>
#include <oid.hpp>
#include <sha512.hpp>
#include <sha384.hpp>
#include <sha224.hpp>
#include <sha256.hpp>
#include <pkcs1v1_5.hpp>

#include <HugeTest.cpp>
#include <HugeCoreTest.cpp>

#include <vector>
#include <random>

using namespace std;

int main(int argc, char** argv) {

    EME_PKCS1_v1_5 eme;
    EMSA_PKCS1_v1_5 emsa;
    
    std::vector<uint8_t> message{'*','*','*','*','*'};

    std::vector<uint8_t> enc1 = eme.Encode(message, 50);
    
    std::vector<uint8_t> enc2 = emsa.Encode<SHA224>(message, 50);
    
    std::copy(enc1.begin(), enc1.end(), std::ostream_iterator<uint16_t>(std::cout, " "));
    std::cout<<"\n";
    
    std::copy(enc2.begin(), enc2.end(), std::ostream_iterator<uint16_t>(std::cout, " "));
    std::cout<<"\n";

    return 0;
    //::testing::InitGoogleTest(&argc, argv);

    // return RUN_ALL_TESTS();
}
