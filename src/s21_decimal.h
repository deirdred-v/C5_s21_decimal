#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum { NEGATIVE = 1, POSITIVE = 0 } sign_t;

typedef enum { ADD = 0, SUB = 1, MUL = 2, DIV = 3 } operation_t;

typedef enum { FULL_DIV = 1, INT_DIV = 2 } div_type_t;

typedef enum { ROUND = 1, BANK_ROUND = 2, FLOOR = 3, TRUNCATE = 4 } round_t;

typedef enum { LESS = -1, EQUAL = 0, GREATER = 1 } compare_t;

typedef struct {
  int bits[7];
} s21_big_decimal;

typedef struct {
  int bits[4];
} s21_decimal;

int s21_set_scale(s21_decimal *dec, int num);
int s21_get_scale(s21_decimal);
void s21_set_sign(s21_decimal *dec, sign_t sign_value);
sign_t s21_get_sign(s21_decimal);
void s21_copy_decimal(s21_decimal source, s21_decimal *destination);
void s21_clear_decimal(s21_decimal *dec);
void s21_big_set_num_bit(s21_big_decimal *dec, int bit_pos, int bit_value);
int s21_big_get_num_bit(s21_big_decimal, int bit_pos);
void s21_big_set_scale(s21_big_decimal *dec, int num);
int s21_big_get_scale(s21_big_decimal);
void s21_big_set_sign(s21_big_decimal *dec, sign_t sign_value);
sign_t s21_big_get_sign(s21_big_decimal);
void s21_copy_big_decimal(s21_big_decimal source, s21_big_decimal *destination);
void s21_clear_big_decimal(s21_big_decimal *dec);
s21_big_decimal s21_create_simple_big_dec(int num);

int s21_big2dec(s21_big_decimal source, s21_decimal *destination);
void s21_dec2big(s21_decimal source, s21_big_decimal *destination);
void s21_big2int(s21_big_decimal source, int *destination);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_big_add(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                s21_big_decimal *big_result);
int s21_big_sub(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                s21_big_decimal *big_result);
int s21_big_mul(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                s21_big_decimal *big_result);
int s21_big_div(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                s21_big_decimal *big_result, int *round_digit_p,
                int *bank_round_digit_p, div_type_t div_type);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_not_equal(s21_decimal, s21_decimal);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);

void s21_bank_round(s21_big_decimal big_dec, s21_big_decimal *big_result);

int s21_negate(s21_decimal value, s21_decimal *result);

int s21_is_not_equal_big(s21_big_decimal dec1, s21_big_decimal dec2);
int s21_is_greater_big(s21_big_decimal dec1, s21_big_decimal dec2);
int s21_pow_ten_big(s21_big_decimal big_dec, int power,
                    s21_big_decimal *big_result);

int s21_shift_bits_left_big(s21_big_decimal *dec, int shift);
void s21_swap_big_decimal(s21_big_decimal *big_dec1, s21_big_decimal *big_dec2);
int process_arithmetic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result,
                       operation_t operation_type);
int s21_process_scale(s21_big_decimal *big_dec1, s21_big_decimal *big_dec2,
                      s21_big_decimal *big_result);

void s21_apply_bank_round_rules(s21_big_decimal *big_dec, int round_digit,
                                int bank_round_digit);
void s21_apply_div_round_rules(s21_big_decimal *big_dec, int round_digit);
void s21_apply_round_rules(s21_big_decimal *big_dec, int round_digit);
void s21_apply_floor_rules(s21_big_decimal *big_dec, int round_digit);

sign_t process_mul_sign(s21_big_decimal *big_dec1, s21_big_decimal *big_dec2);
int abs_equal_zero(s21_big_decimal big_dec);
int s21_dec2int(s21_decimal source, int *destination);
#endif