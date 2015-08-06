/*
 * File:   algorithm.h
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:41
 */

#ifndef HUGE_H
#define HUGE_H

typedef unsigned char byte;

byte* uadd(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

byte* usub(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

//byte* usub_strong(bool& neg, byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

byte* umul(byte* first_result, byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

byte* udiv(byte* div_first, byte* div_last, byte* rem_first, byte* rem_last, const byte* first1, const byte* last1, const byte* first2,
           const byte* last2);

//byte* ssub(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

byte* uincrement(byte* first1, byte* last1);

byte* udecrement(byte* first1, byte* last1);

bool is_even(byte* first, byte* last);

bool is_odd(byte* first, byte* last);

void reverse(byte* first, byte* last);

void rotate_left(byte* first, byte* last);

void rotate_right(byte* first, byte* last);

void shift_right(byte* first, byte* last);

void shift_left(byte* first, byte* last);

void rotate_left_n(byte* first, byte* last, int n);

void rotate_right_n(byte* first, byte* last, int n);

void shift_left_n(byte* first, byte* last, int n);

void shift_right_n(byte* first, byte* last, int n);

void op_or(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void op_xor(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void op_and(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void op_not(byte* first, byte* last);

short ucmp(const byte* first1, const byte* last1, const byte* first2, const byte* last2);

bool is_zero(const byte* first, const byte* last);

int get_low_bit(const byte* first, const byte* last);

int get_high_bit(const byte* first, const byte* last);

#endif   /* ALGORITHM_H */

