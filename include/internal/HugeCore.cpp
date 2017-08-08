
#include "HugeCore.h"

#include <cstdio>
#include <iterator>
#include <vector>
#include <cstdint>

using namespace std;

static void dump(const byte* f, const byte* l) {
    for (; f != l; ++f) {
        printf("%02x ", *f);
    }
    printf("\n");
}

void HUGE_Add(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    byte carry = 0;
    unsigned short tmp = 0;

    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2) {
        tmp = (*(last1)) + (*(last2)) + (carry);
        *(--result) = (byte)tmp;
        carry = tmp >> 8;
    }

    for (; first1 <= last1; --last1) {
        tmp = (*last1) + (carry);
        *(--result) = (byte)tmp;
        carry = tmp >> 8;
    }

    for (; first2 <= last2; --last2) {
        tmp = (*last2) + (carry);
        *(--result) = (byte)tmp;
        carry = tmp >> 8;
    }
}

void HUGE_Subtract(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    unsigned short carry = 0;

    --last1;
    --last2;

    for (; (last1 >= first1) && (last2 >= first2); --last1, --last2) {
        if (*(last1) < *(last2) + carry) {
            *(--result) = (*last1) - (*last2) - carry + 256;
            carry = 1;
        } else {
            *(--result) = (*last1) - (*last2) - carry;
            carry = 0;
        }
    }

    for (; first1 <= last1; --last1) {
        *(--result) = *(last1)-carry;
        carry = 0;
    }
}

void HUGE_Multiply(byte* first_result, byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    --last1;
    --last2;

    byte carry = 0x00;
    byte* resultIter = nullptr;

    for (; first2 <= last2; --last2) {
        --last_result;

        resultIter = last_result;

        for (const byte* last_1 = last1; first1 <= last_1; --last_1) {
            uint16_t temp = 0x0000;

            temp = (*resultIter) + (*last_1) * (*last2) + carry;

            carry = temp / 256;

            *(resultIter) = temp % 256;

            --resultIter;
        }

        *(resultIter) = carry;
        carry = 0x00;
    }
}

void HUGE_Multiply(byte* first_result, byte* last_result, const byte* first1, const byte* last1, byte x) {
    --last1;
    --last_result;

    uint16_t temp = 0;
    uint8_t carry = 0x00;

    for (; first1 <= last1; --last1) {
        temp = (*last1) * x + carry;

        carry = temp / 256;
        *(last_result) = temp % 256;

        --last_result;
    }

    *last_result = carry;
}

void HUGE_DivRem(byte* div_first, byte* div_last, byte* rem_first, byte* rem_last, const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    byte* rFirst = nullptr;
    byte* rLast = nullptr;
    const byte* dFirst = nullptr;
    const byte* dLast = nullptr;

    while ((first1 != last1) && (*first1 == 0x00)) {
        ++first1;
    }

    while ((first2 != last2) && (*first2 == 0x00)) {
        ++first2;
    }

    auto rTmp = std::vector<byte>(first1, last1);

    auto d1 = std::distance(first1, last1);
    auto d2 = std::distance(first2, last2);
    auto shift = d1 - d2 + 1;
    rFirst = &rTmp[0];
    rLast = rFirst + d2;
    dFirst = first2;
    dLast = last2;

    std::advance(div_last, -shift);

    auto cmp = HUGE_Compare(rFirst, rLast, dFirst, dLast);
    if (cmp == -1) {
        ++rLast;
        ++div_last;
        --shift;
    }

    size_t nbytes = d2 + 1;
    std::vector<byte> mul(nbytes);

    while (shift > 0) {
        uint8_t Down = 0x00;
        uint16_t Up = 0x00100;
        uint8_t Middle = 0x00;

        auto cmp = HUGE_Compare(rFirst, rLast, dFirst, dLast);
        if (cmp == -1) {
            ++rLast;
        }

        for (; Down < Up - 1;) {
            // 1. c <-- (down + up) / 2;
            Middle = ((Down + Up) / 2);

            // 2. mul <-- d * c;
            std::fill(&mul[0], &mul[0] + nbytes, 0x00);

            HUGE_Multiply(&mul[0], &mul[0] + nbytes, dFirst, dLast, Middle);

            short mulCmp = HUGE_Compare(&mul[0], &mul[0] + nbytes, rFirst, rLast);

            if (mulCmp == -1) { // if(c < a): down <-- c
                Down = Middle;
            } else if (mulCmp == 1) { // if(c > a) Up <-- c
                Up = Middle;
            } else if (mulCmp == 0) { // if(mul == a) Up <-- C; Down <-- Up;
                Up = Middle;
                Down = (uint8_t)Up;
            }
        }

        std::fill(&mul[0], &mul[0] + nbytes, 0x00);

        HUGE_Multiply(&mul[0], &mul[0] + nbytes, dFirst, dLast, Down);

        HUGE_Subtract(rLast, rFirst, rLast, &mul[0], &mul[0] + nbytes);

        *(div_last++) = Down;

        --shift;
    }

    std::copy_backward(rFirst, rLast, rem_last);
}

void HUGE_Increment(byte* first, byte* last) {
    unsigned short tmp = 0x00;
    byte carry = 0x00;

    tmp = (*--last) + 0x01 + carry;
    *(last) = (byte)tmp;
    carry = tmp >> 8;

    for (; (first <= last) && carry;) {
        tmp = (*--last) + carry;
        *(last) = (byte)tmp;
        carry = tmp >> 8;
    }
}

void HUGE_Decrement(byte* first, byte* last) {
    unsigned short carry = 0;

    if (*--last < 0x01) {
        *(last) = (*last) - (0x01) - carry + 256;
        carry = 1;
    } else {
        *(last) = (*last) - (0x01) - carry;
        carry = 0;
    }

    for (; (first <= last);) {
        if (*--last < carry) {
            *(last) = (*last) - carry + 256;
            carry = 1;
        } else {
            *(last) = (*last) - carry;
            carry = 0;
        }
    }
}

bool HUGE_IsOdd(const byte* first, const byte* last) { return (*(--last) & 0x01) == 0x01; }

bool HUGE_IsEven(const byte* first, const byte* last) { return (*(--last) & 0x01) == 0x00; }

void HUGE_Reverse(byte* first, byte* last) { std::reverse(first, last); }

void HUGE_RotateLeft(byte* first, byte* last) {
    int carry = (*first >> 7) & 0x01;

    for (; first != last; --last) {
        int z = (*last >> 7) & 0x01;

        *last <<= 1;
        *last |= (byte)carry;
        carry = z;
    }
}

void HUGE_RotateRight(byte* first, byte* last) {
    int carry = *(last - 1) & 0x01; // *(std::prev(last)) & 0x01;

    for (; first != last; ++first) {
        int z = *first & 0x01;

        *first >>= 1;
        *first |= (byte)(carry << 7);
        carry = z;
    }
}

void HUGE_ShiftRight(byte* first, byte* last) {
    int carry = 0;

    for (; first != last; ++first) {
        int z = (*first) & 0x01;

        *first >>= 1;
        *first |= (byte)(carry << 7);
        carry = z;
    }
}

void HUGE_ShiftLeft(byte* first, byte* last) {
    int carry = 0;

    --last;

    for (; first <= last; --last) {
        int z = (*(last) >> 7) & 0x01;

        *last <<= 1;
        *last |= (byte)carry;
        carry = z;
    }
}

void HUGE_RotateLeft(byte* first, byte* last, int n) {
    while (n--) {
        HUGE_RotateLeft(first, last);
    }
}

void HUGE_RotateRight(byte* first, byte* last, int n) {
    --last;

    while (n--) {
        HUGE_RotateRight(first, last);
    }
}

void HUGE_ShiftLeft(byte* first, byte* last, int n) {
    while (n--) {
        HUGE_ShiftLeft(first, last);
    }
}

void HUGE_ShiftRight(byte* first, byte* last, int n) {
    while (n--) {
        HUGE_ShiftRight(first, last);
    }
}

void HUGE_Or(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2) {
        *(--result) = *(last1) | *(last2);
    }

    for (; first1 <= last1; --last1) {
        *(--result) = *last1;
    }

    for (; first2 <= last2; --last2) {
        *(--result) = *last2;
    }
}

void HUGE_Xor(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2) {
        *(--result) = (*last1) ^ (*last2);
    }

    for (; first1 <= last1; --last1) {
        *(--result) = *last1;
    }

    for (; first2 <= last2; --last2) {
        *(--result) = *last2;
    }
}

void HUGE_And(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2) {
        *(--result) = *(last1) & *(last2);
    }

    for (; first1 <= last1; --last1) {
        *(--result) = 0x00;
    }

    for (; first2 <= last2; --last2) {
        *(--result) = 0x00;
    }
}

void HUGE_Inverse(byte* first, byte* last) {
    for (; first != last; ++first) {
        *first = ~(*first);
    }
}

short HUGE_Compare(const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    for (; (first1 != last1) && (*first1 == 0);) {
        ++first1;
    }

    for (; (first2 != last2) && (*first2 == 0);) {
        ++first2;
    }

    for (; (first1 != last1) && (first2 != last2) && (*first1 == *first2); ++first1, ++first2)
        ;

    auto dA = std::distance(first1, last1);
    auto dB = std::distance(first2, last2);

    if (dA == dB) {
        if ((first1 < last1) && (first2 < last2)) {
            return (short)((*first1 < *first2) ? -1 : 1);
        }
    }

    if (dA < dB) {
        return -1;
    }

    if (dA > dB) {
        return 1;
    }

    return 0;
}

bool HUGE_Equal(const byte* first1, const byte* last1, const byte* first2, const byte* last2) {
    bool ret = false;

    for (; (first1 != last1) && (*first1 == 0x00); ++first1)
        ;

    for (; (first2 != last2) && (*first2 == 0x00); ++first2)
        ;

    for (; (first1 != last1) && (first2 != last2) && (*first1) == (*first2); ++first1, ++first2)
        ;

    return (first1 == last1) && (first2 == last2);
}

bool HUGE_IsZero(const byte* first, const byte* last) {
    for (; (first != last) && (*first == 0x00);) {
        ++first;
    }

    return first == last;
}

int HUGE_GetLowestSetBit(const byte* first, const byte* last) { return ((*std::prev(last)) & 0x01); }

int HUGE_GetHighestSetBit(const byte* first, const byte* last) { return (*first & 0x80); }

bool HUGE_isPowerOfTwo(const byte* first, const byte* last) { return false; }

bool HUGE_IsOne(const byte* first, const byte* last) {
    --last;

    if (*last == 0x01) {
        --last;

        while ((first <= last) && (*last == 0x00)) {
            --last;
        }

        return !(first <= last);
    }

    return false;
}
