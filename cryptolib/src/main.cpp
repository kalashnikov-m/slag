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
#include <pss.hpp>
#include <oaep.hpp>
#include <mgf.hpp>

#include <HugeTest.cpp>
#include <HugeCoreTest.cpp>

#include <vector>
#include <random>

using namespace std;

int main(int argc, char** argv) {

    const uint8_t M[] = {0xd4, 0x36, 0xe9, 0x95, 0x69, 0xfd, 0x32, 0xa7, 0xc8, 0xa0, 0x5b, 0xbc, 0x90, 0xd3, 0x2c, 0x49};

    size_t modBits = 1024;
    size_t emBits = (modBits - 1) % 8 == 0 ? (modBits - 1) : modBits;
    size_t emLen = emBits / 8;

    std::vector<uint8_t> encode(emLen);

    cry::OAEP<SHA1> oaep;
    oaep.Encode(M, M + sizeof(M), encode.begin(), emLen);

    for (int i = 0; i < emLen; ++i) {
        printf("%02x ", encode[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");

    std::vector<uint8_t> D(sizeof(M));
    oaep.Decode(encode.begin(), encode.end(), D.begin(), emLen);

    for (int i = 0; i < sizeof(M); ++i) {
        printf("%02x ", D[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");

    /*const uint8_t M[] = {0x85, 0x9e, 0xef, 0x2f, 0xd7, 0x8a, 0xca, 0x00, 0x30, 0x8b, 0xdc, 0x47, 0x11, 0x93, 0xbf, 0x55, 0xbf, 0x9d, 0x78, 0xdb, 0x8f, 0x8a, 0x67, 0x2b, 0x48, 0x46, 0x34, 0xf3, 0xc9, 0xc2, 0x6e, 0x64, 0x78, 0xae, 0x10, 0x26, 0x0f, 0xe0,
                         0xdd, 0x8c, 0x08, 0x2e, 0x53, 0xa5, 0x29, 0x3a, 0xf2, 0x17, 0x3c, 0xd5, 0x0c, 0x6d, 0x5d, 0x35, 0x4f, 0xeb, 0xf7, 0x8b, 0x26, 0x02, 0x1c, 0x25, 0xc0, 0x27, 0x12, 0xe7, 0x8c, 0xd4, 0x69, 0x4c, 0x9f, 0x46, 0x97, 0x77, 0xe4, 0x51,
                         0xe7, 0xf8, 0xe9, 0xe0, 0x4c, 0xd3, 0x73, 0x9c, 0x6b, 0xbf, 0xed, 0xae, 0x48, 0x7f, 0xb5, 0x56, 0x44, 0xe9, 0xca, 0x74, 0xff, 0x77, 0xa5, 0x3c, 0xb7, 0x29, 0x80, 0x2f, 0x6e, 0xd4, 0xa5, 0xff, 0xa8, 0xba, 0x15, 0x98, 0x90, 0xfc};

    cry::PSS<SHA1> pss;

    size_t modBits = 1024;
    size_t emBits = (modBits - 1) % 8 == 0 ? (modBits - 1) : modBits;
    size_t emLen = emBits / 8;

    std::vector<uint8_t> encode(emLen);

    pss.Encode(M, M + sizeof(M), encode.begin(), emBits);

    for (int i = 0; i < emLen; ++i) {
        printf("%02x ", encode[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");

    // std::vector<uint8_t> decode(emLen);
    std::cout << pss.Verify(std::begin(M), std::end(M), encode.begin(), encode.end(), emBits);*/

    return 0;
    //::testing::InitGoogleTest(&argc, argv);

    // return RUN_ALL_TESTS();
}
