/*
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include "algorithm.hpp"
#include "googletest/include/gtest/gtest.h"
#include <thread>

namespace myNamespace
{
    template <class T>
    void generate_key_pair(T& n, T& d, uint32_t e, uint32_t modulusbits)
    {
        T p, q;
        cry::generate_probably_prime<T>(std::ref(p), modulusbits / 2, e);
        cry::generate_probably_prime<T>(std::ref(q), modulusbits / 2, e);

        std::cout << "p: " << p << std::endl;
        std::cout << "q: " << q << std::endl;

        T N = p * q;

        std::cout << "N: " << N << std::endl;

        T Phi = (p - 1) * (q - 1);
        std::cout << "Phi: " << Phi << std::endl;

        T D;
        const bool f = cry::mod_inverse(D, T(e), Phi);
        if (!f)
        {
            throw std::runtime_error("invserse does not exists");
        }

        std::cout << "D: " << D << std::endl;

        n = N;
        d = D;
    }

    template <class T>
    void generate_key_pair_mt(T& n, T& d, uint32_t e, uint32_t modulusbits)
    {
        T p, q;

        std::thread pThread(cry::generate_probably_prime<T>, std::ref(p), modulusbits / 2, e);
        std::thread qThread(cry::generate_probably_prime<T>, std::ref(q), modulusbits / 2, e);

        pThread.join();
        qThread.join();

        std::cout << "p: " << p << std::endl;
        std::cout << "q: " << q << std::endl;

        T N = p * q;

        std::cout << "N: " << N << std::endl;

        T Phi = (p - 1) * (q - 1);
        std::cout << "Phi: " << Phi << std::endl;

        T D;
        const bool f = cry::mod_inverse(D, T(e), Phi);
        if (!f)
        {
            throw std::runtime_error("invserse does not exists");
        }

        std::cout << "D: " << D << std::endl;

        n = N;
        d = D;
    }
}

int main(int argc, char** argv)
{

    ::testing::InitGoogleTest(&argc, argv);

    int rval = RUN_ALL_TESTS();

    return rval;
}
