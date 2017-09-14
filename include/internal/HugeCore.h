/*
 * File:   algorithm.h
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:41
 */

#ifndef HUGE_CORE_H
#define HUGE_CORE_H

typedef unsigned char byte;

void Cry_add(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void Cry_subtract(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void Cry_multiply(byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void Cry_multiply(byte* last_result, const byte* first1, const byte* last1, byte x);

void Cry_div_rem(byte* div_last, byte* rem_last, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void Cry_increment(byte* first1, byte* last1);

void Cry_decrement(byte* first1, byte* last1);

bool Cry_is_even(const byte* first, const byte* last);

bool Cry_is_odd(const byte* first, const byte* last);

bool Cry_is_one(const byte* first, const byte* last);

bool Cry_is_zero(const byte* first, const byte* last);

void Cry_reverse(byte* first, byte* last);

void Cry_rotl(byte* first, byte* last);

void Cry_rotl(byte* first, byte* last, int n);

void Cry_rotr(byte* first, byte* last);

void Cry_rotr(byte* first, byte* last, int n);

void Cry_rshift(byte* first, byte* last);

void Cry_rshift(byte* first, byte* last, int n);

void Cry_lshift(byte* first, byte* last);

void Cry_lshift(byte* first, byte* last, int n);

void Cry_or(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void Cry_xor(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void Cry_and(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void Cry_inverse(byte* first, byte* last);

short Cry_compare(const byte* first1, const byte* last1, const byte* first2, const byte* last2);

bool Cry_equal(const byte* first1, const byte* last1, const byte* first2, const byte* last2);

int Cry_get_lowest_set_bit(const byte* first, const byte* last);

int Cry_get_highest_set_bit(const byte* first, const byte* last);

// TODO isPowerOfTwo()

#endif /* ALGORITHM_H */
