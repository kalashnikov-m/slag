#include "huge.h"

#include <cstdio>
#include <vector>
#include <algorithm>

static void dump(const byte* b, const byte* c)
{
    while (b != c) {
        printf("%02x:", *b);
        ++b;
    }

    printf("\n");
}

#define BUF_SIZE 8

static int heighBitIdx(byte x)
{
    int result = -1;

    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;

    x = x - (x >> 1);

    for (; x != 0; x >>= 1)
        ++result;

    return result;
}

byte* uadd(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    byte carry = 0;
    unsigned short tmp = 0;

    --last1, --last2;

    for (; first1 <= last1 && first2 <= last2; --last1, --last2) {
        tmp = (*last1) + (*last2) + (carry);
        *(--result) = (byte) tmp;
        carry = tmp >> 8;
    }

    for (; first1 <= last1; --last1) {
        tmp = (*last1) + (carry);
        *(--result) = (byte) tmp;
        carry = tmp >> 8;
    }

    for (; first2 <= last2; --last2) {
        tmp = (*last2) + (carry);
        *(--result) = (byte) tmp;
        carry = tmp >> 8;
    }

    return result;
}

byte * usub(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte * last2)
{
    unsigned short carry = 0;

    --last1, --last2;

    for (; last1 >= first1 && last2 >= first2; --last1, --last2) {

        if (*last1 < *last2 + carry) {
            *(--result) = (*last1) - (*last2) - carry + 256;
            carry = 1;
        } else {
            *(--result) = (*last1) - (*last2) - carry;
            carry = 0;
        }
    }

    for (; first1 <= last1; --last1, --result) {
        *(--result) = *last1 - carry;
        carry = 0;
    }

    return result;
}

byte* usub_strong(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{

}

byte * umul(byte* first_result, byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte * last2)
{
    byte a[BUF_SIZE] = {0x00};
    byte b[BUF_SIZE] = {0x00};
    byte* iter = nullptr;

    std::copy_backward(first1, last1, std::end(a));
    std::copy_backward(first2, last2, std::end(b));

    while (!is_zero(std::begin(b), std::end(b))) {

        if (get_low_bit(std::begin(b), std::end(b)) == 0x01) {

            iter = uadd(last_result, first_result, last_result, std::begin(a), std::end(a));
            dump(first_result, last_result);
        }

        shift_left(std::begin(a), std::end(a));
        shift_right(std::begin(b), std::end(b));
    }

    return iter;
}

byte * udiv(byte* div_first, byte* div_last, 
        byte* rem_first, byte* rem_last,
        const byte* first1, const byte* last1,
        const byte* first2, const byte* last2)
{
    byte Down[BUF_SIZE] = {0x00}; // 
    byte Up[BUF_SIZE] = {0x00}; // 
    byte C[BUF_SIZE] = {0x00};
    byte r[BUF_SIZE] = {0x00}; // делимое
    byte d[BUF_SIZE] = {0x00}; // делитель
    byte mul[BUF_SIZE] = {0x00}; // 
    
    const byte* a1 = nullptr;
    const byte* a2 = nullptr;
    const byte* b1 = nullptr;
    const byte* b2 = nullptr;

    while(first1 != last1 && *first1 == 0x00)
        ++first1;
    
    while(first2 != last2 && *first2 == 0x00)
        ++first2;
    
    //dump(first1, last1);
    //dump(first2, last2);
    
    auto d1 = std::distance(first1, last1);
    auto d2 = std::distance(first2, last2);

    auto shift = d1 - d2;

    b1 = first2;
    b2 = last2;

    *(std::prev(std::prev(std::end(Up)))) = 0x01; // Up <-- 256;

    while(shift > 0)
    {
	a1 = first1; a2 = a1 + d2;
	
	//dump(a1, a2);
	//dump(b1, b2);
	auto cmp = ucmp(a1, a2, b1, b2);
	if(cmp == -1)
	{	
	    ++a2;
	}

	//dump(a1, a2);
	
	std::copy_backward(a1, a2, std::end(r)); // r <-- a
	std::copy_backward(b1, b2, std::end(d)); // d <-- b

	//dump(std::begin(r), std::end(r));
	auto updown_cmp = ucmp(std::begin(Down), std::end(Down), std::begin(Up), std::end(Up));
	for(;updown_cmp == -1;) {
	    
	    // 1. c <-- (down + up);
    	    uadd(std::end(C), std::begin(Down), std::end(Down), std::begin(Up), std::end(Up));

	    // 2. c <-- c / 2;
    	    shift_right(std::begin(C), std::end(C));

	    // 3. mul <-- d * c;
    	    umul(std::begin(mul), std::end(mul), std::begin(d), std::end(d), std::begin(C), std::end(C));

	    short mulr_cmp = ucmp(std::begin(mul), std::end(mul), std::begin(r), std::end(r));

	    if (mulr_cmp == -1)
	    {
	    // if(c < a)
            //uadd(std::end(down), std::begin(down), std::end(down), std::begin(c), std::end(c));
    	    } else if (mulr_cmp == 1)
	    { // if(c >= a)
            //usub(std::end(up), std::begin(up), std::end(up), std::begin(c), std::end(c));
    	    } else if(mulr_cmp ==0)
	    {
	
	    }
	}

	--shift;
    }
    
    
    /*std::prev(std::prev(std::end(up))) = 0x01; // up = 256;

    std::copy_backward(first1, last1, std::end(r));
    std::copy_backward(first2, last2, std::end(d));

    for (; ucmp(std::begin(down), std::end(down), std::begin(up), std::end(up)) == -1;) {

        // 1. c = (down + up);
        uadd(std::end(c), std::begin(down), std::end(down), std::begin(up), std::end(up)); 

        // 2. c = c / 2;
        shift_right(std::begin(c), std::end(c)); // c = c / 2;

        // 3. d * c;
        umul(std::begin(mul), std::end(mul), std::begin(d), std::end(d), std::begin(c), std::end(c));

	short cmp = ucmp(std::begin(mul), std::end(mul), std::begin(r), std::end(r));
        if (cmp == -1)
	{
	 // if(c < a)
            //uadd(std::end(down), std::begin(down), std::end(down), std::begin(c), std::end(c));
        } else if (cmp == 1)
	{ // if(c >= a)
            //usub(std::end(up), std::begin(up), std::end(up), std::begin(c), std::end(c));
        } else if(cmp ==0)
	{
	
	}
    }*/

    return nullptr;
}

void rotate_left(byte* first, byte * last)
{
    int carry = (*first >> 7) & 0x01;

    for (; first != last; --last) {

        int z = (*last >> 7) & 0x01;
        *last <<= 1;
        *last |= (byte) carry;
        carry = z;
    }
}

void rotate_right(byte* first, byte * last)
{
    int carry = *(last - 1) & 0x01; // *(std::prev(last)) & 0x01;

    for (; first != last; ++first) {

        int z = *first & 0x01;
        *first >>= 1;
        *first |= (byte) (carry << 7);
        carry = z;
    }
}

void shift_right(byte* first, byte * last)
{
    int carry = 0;

    for (; first != last; ++first) {

        int z = (*first) & 0x01;
        *first >>= 1;
        *first |= (byte) (carry << 7);
        carry = z;
    }
}

void shift_left(byte* first, byte * last)
{
    int carry = 0;

    --last;

    for (; first <= last; --last) {

        int z = (*last >> 7) & 0x01;
        *last <<= 1;
        *last |= (byte) carry;
        carry = z;
    }
}

void rotate_left_n(byte* first, byte* last, int n)
{
    while (n--) {

        rotate_left(first, last);
    }
}

void rotate_right_n(byte* first, byte* last, int n)
{
    --last;
    while (n--) {

        rotate_right(first, last);
    }
}

void shift_left_n(byte* first, byte* last, int n)
{
    while (n--) {

        shift_left(first, last);
    }
}

void shift_right_n(byte* first, byte* last, int n)
{
    while (n--) {

        shift_right(first, last);
    }
}

void op_or(byte* result, const byte * first1, const byte* last1, const byte* first2, const byte * last2)
{
    for (; first1 <= last1 && first2 <= last2; --last1, --last2, --result) {
        *result = *last1 | *last2;
    }

    for (; first1 <= last1; --last1, --result)
        *result = *first1;

    for (; first2 <= last2; --last2, --result)
        *result = *first2;
}

void op_xor(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte * last2)
{
    --last1, --last2, --result;
    for (; first1 != last1 && first2 <= last2; --last1, --last2, --result) {
        (*result) = (*last1--) ^ (*last2);
    }

    for (; first1 <= last1; --last1, --result)
        (*result) = (*first1);

    for (; first2 <= last2; --last2, --result)
        (*result) = (*first2);
}

void op_and(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte * last2)
{
    --last1, --last2, --result;

    for (; first1 <= last1 && first2 <= last2; --last1, --last2, --result) {
        *result-- = *last1-- & *last2--;
    }

    for (; first1 != last1; --last1, --result)
        *result = 0x00; //*first1;

    for (; first2 != last2; --last2, --result)
        *result = 0x00; //*first2;
}

void op_not(byte* first, byte * last)
{
    for (; first != last; ++first) {

        *first = ~(*first);
    }
}

short ucmp(const byte* first1, const byte* last1, const byte* first2, const byte * last2)
{
    for (; first1 != last1 && (*first1 == 0); ++first1);
    for (; first2 != last2 && (*first2 == 0); ++first2);

    for (; first1 != last1 && first2 != last2 && *first1 == *first2; ++first1, ++first2);

    auto dA = std::distance(first1, last1);
    auto dB = std::distance(first2, last2);

    if (dA == dB)
        if (first1 < last1 && first2 < last2)
            return (short) (*first1 < *first2 ? -1 : 1);

    if (dA < dB)
        return -1;

    if (dA > dB)

        return 1;

    return 0;
}

bool is_zero(const byte* first, const byte * last)
{
    for (; first != last && *first == 0x00; ++first);

    return first == last;
}

int get_low_bit(const byte* first, const byte * last)
{
    return ((*std::prev(last)) & 0x01);
}

int get_high_bit(const byte* first, const byte * last)
{
    return (*first & 0x80);
}

/*byte* clone(byte* result, const byte* first, const byte* last)
{
    result = std::copy(first, last, result);

    return result;
}*/