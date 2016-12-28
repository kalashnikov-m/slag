/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include "gtest/gtest.h"

#include <algorithm.hpp>
#include <sha1.hpp>

#include <HugeTest.cpp>
#include <HugeCoreTest.cpp>

#include <vector>

using namespace std;

int main(int argc, char** argv) {
    const uint8_t m1[] = {"abc"};
    const uint8_t m2[] = {""};
    const uint8_t m3[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
    const uint8_t m4[] = {"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijkl"
                          "mnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopq"
                          "rstu"};

    std::vector<uint8_t> digest;

    SHA1 hash;

    hash.Init();
    hash.Update(std::vector<uint8_t>(m1, m1 + sizeof(m1) / sizeof(uint8_t) - 1));
    hash.Final(digest);

    std::for_each(digest.begin(), digest.end(), [](uint8_t b) { printf("%02x", b); });
    std::cout << "\n";

    hash.Init();
    hash.Update(std::vector<uint8_t>(m2, m2 + sizeof(m2) / sizeof(uint8_t) - 1));
    hash.Final(digest);

    std::for_each(digest.begin(), digest.end(), [](uint8_t b) { printf("%02x", b); });
    std::cout << "\n";

    hash.Init();
    hash.Update(std::vector<uint8_t>(m3, m3 + sizeof(m3) / sizeof(uint8_t) - 1));
    hash.Final(digest);

    std::for_each(digest.begin(), digest.end(), [](uint8_t b) { printf("%02x", b); });
    std::cout << "\n";

    hash.Init();
    hash.Update(std::vector<uint8_t>(m4, m4 + sizeof(m4) / sizeof(uint8_t) - 1));
    hash.Final(digest);

    std::for_each(digest.begin(), digest.end(), [](uint8_t b) { printf("%02x", b); });
    std::cout << "\n";

    // return 0;

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
