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

#include "HugeCoreTest.cpp"
#include "HugeTest.cpp"

#include <Huge.hpp>

#include <vector>
#include <random>

#include <locale>
#include <cctype>

#include <algorithm>
#include <functional>
#include <rsa/rsa.hpp>

#include "googletest/include/gtest/gtest.h"

using namespace std;
using namespace cry;

int main(int argc, char** argv) {

/*        cry::bigint8_t p, q;

        p = cry::generate_probably_prime<cry::bigint8_t>(512, 17);
        q = cry::generate_probably_prime<cry::bigint8_t>(512, 17);

        std::cout<<"p: "<<p<<std::endl;
        std::cout<<"q: "<<q<<std::endl;
    }*/

    //return 0;

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
