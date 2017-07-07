/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

//#include "gtest/gtest.h"

#include <algorithm.hpp>
#include <sha1.hpp>
#include <oid.hpp>
#include <sha512.hpp>
#include <sha384.hpp>
#include <sha224.hpp>
#include <sha256.hpp>
#include <pkcs1v1_5.hpp>
#include <pss.hpp>
#include <oaep.hpp>
#include <mgf.hpp>

//#include <HugeTest.cpp>
//#include <HugeCoreTest.cpp>
#include <Huge.hpp>

#include <vector>
#include <random>

#include <locale>
#include <cctype>

#include <algorithm>
#include <functional>

using namespace std;

std::function<void(const std::vector<uint8_t>&)> trace = [](const std::vector<uint8_t>& container) {
    for (size_t i = 0; i < container.size(); ++i) {
        printf("%02x ", container[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("\n\n");
};

template <class T, class E>
void GenerateKeyPair(T l, E e) {
    //(l-1)/2 - 1;
}

int main(int argc, char** argv) {

    cry::bigint8_t p, q;

    p = cry::generate_probably_prime<cry::bigint8_t>(512, 17);
    q = cry::generate_probably_prime<cry::bigint8_t>(512, 17);

    std::cout<<"p: "<<p<<std::endl;
    std::cout<<"q: "<<q<<std::endl;

    return 0;
    //::testing::InitGoogleTest(&argc, argv);

    //return RUN_ALL_TESTS();
}
