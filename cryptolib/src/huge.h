/*
 * File:   algorithm.h
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:41
 */

#ifndef HUGE_H
#define HUGE_H

typedef unsigned char byte;

void HUGE_Add(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void HUGE_Subtract(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void HUGE_Multiply(byte* first_result, byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void HUGE_DivRem(byte* div_first, byte* div_last, byte* rem_first, byte* rem_last, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void HUGE_Increment(byte* first1, byte* last1);

void HUGE_Decrement(byte* first1, byte* last1);

bool HUGE_IsEven(byte* first, byte* last);

bool HUGE_IsOdd(byte* first, byte* last);

void HUGE_Reverse(byte* first, byte* last);

void HUGE_RotateLeft(byte* first, byte* last);

void HUGE_RotateRight(byte* first, byte* last);

void HUGE_ShiftRight(byte* first, byte* last);

void HUGE_ShiftLeft(byte* first, byte* last);

void HUGE_RotateLeftN(byte* first, byte* last, int n);

void HUGE_RotateRightN(byte* first, byte* last, int n);

void HUGE_ShiftLeftN(byte* first, byte* last, int n);

void HUGE_ShiftRightN(byte* first, byte* last, int n);

void HUGE_Or(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void HUGE_Xor(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void HUGE_And(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2);

void HUGE_Not(byte* first, byte* last);

short HUGE_Compare(const byte* first1, const byte* last1, const byte* first2, const byte* last2);

bool HUGE_IsZero(const byte* first, const byte* last);

int HUGE_GetLowestSetBit(const byte* first, const byte* last);

int HUGE_GetHighestSetBit(const byte* first, const byte* last);

// TODO HUGE_isOne(), isPowerOfTwo()

#endif   /* ALGORITHM_H */

