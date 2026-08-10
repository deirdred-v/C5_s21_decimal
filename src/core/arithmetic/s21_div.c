#include "../../s21_decimal.h"

void append_bit_to_big_decimal(int bit, s21_big_decimal *dec) {
  s21_shift_bits_left_big(dec, 1);
  s21_big_set_num_bit(dec, 0, bit);
}

int process_div_scale(s21_big_decimal *big_dec1, s21_big_decimal *big_dec2) {
  int current_scale = 0;
  int scale1 = s21_big_get_scale(*big_dec1);
  int scale2 = s21_big_get_scale(*big_dec2);
  if (scale1 > scale2) {
    current_scale = scale1 - scale2;
  } else if (scale1 < scale2) {
    s21_pow_ten_big(*big_dec1, scale2 - scale1, big_dec1);
  }
  s21_big_set_scale(big_dec1, 0);
  s21_big_set_scale(big_dec2, 0);
  return current_scale;
}

void int_div(s21_big_decimal dividend, s21_big_decimal divisor,
             s21_big_decimal *big_result, s21_big_decimal *remainder) {
  s21_big_decimal minuend;
  s21_clear_big_decimal(&minuend);
  s21_clear_big_decimal(big_result);
  for (int i = 191; i >= 0; i--) {
    int dividend_bit = s21_big_get_num_bit(dividend, i);
    append_bit_to_big_decimal(dividend_bit, &minuend);
    if (s21_is_greater_big(divisor, minuend)) {
      append_bit_to_big_decimal(0, big_result);
    } else {
      append_bit_to_big_decimal(1, big_result);
      s21_big_sub(minuend, divisor, &minuend);
    }
  }
  s21_copy_big_decimal(minuend, remainder);
}

int s21_big_div(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                s21_big_decimal *big_result, int *round_digit_p,
                int *bank_round_digit_p, div_type_t div_type) {
  s21_clear_big_decimal(big_result);
  sign_t result_sign = process_mul_sign(&big_dec1, &big_dec2);
  int current_scale = process_div_scale(&big_dec1, &big_dec2);
  int round_digit = 0;
  int bank_round_digit = 0;
  s21_big_decimal temp_result;
  s21_clear_big_decimal(&temp_result);
  s21_big_decimal remainder;
  s21_clear_big_decimal(&remainder);
  s21_big_decimal zero_dec;
  s21_clear_big_decimal(&zero_dec);
  int is_first_div = 1;
  int overflow = 0;
  int one_more_iteration = 0;
  int error = 0;
  do {
    if (one_more_iteration) one_more_iteration = 0;
    int_div(big_dec1, big_dec2, &temp_result, &remainder);
    s21_pow_ten_big(remainder, 1, &remainder);
    s21_copy_big_decimal(remainder, &big_dec1);
    s21_big_set_scale(&temp_result, current_scale);
    current_scale++;

    s21_big_decimal round_digit_big_dec;
    s21_copy_big_decimal(temp_result, &round_digit_big_dec);
    s21_big_set_scale(&round_digit_big_dec, 0);
    s21_big2int(round_digit_big_dec, &round_digit);
    if (round_digit_p != NULL && !is_first_div) *round_digit_p = round_digit;
    if (bank_round_digit_p != NULL && !is_first_div)
      *bank_round_digit_p = bank_round_digit;

    if (!error) error = s21_big_add(temp_result, *big_result, &temp_result);
    s21_decimal temp_buffer;
    if (!error) error = s21_big2dec(temp_result, &temp_buffer);
    if (!error) error = s21_big_get_scale(temp_result) > 28;
    if (error) overflow = 1;

    if (!error) bank_round_digit = round_digit;
    if (!error || is_first_div) s21_copy_big_decimal(temp_result, big_result);
    if (is_first_div) {
      is_first_div = 0;
      if (div_type == INT_DIV) {
        error = 1;
        one_more_iteration = 1;
      }
      if (big_result->bits[0] % 2 == 0) {
        bank_round_digit = 0;
        if (bank_round_digit_p != NULL) *bank_round_digit_p = bank_round_digit;
      } else {
        bank_round_digit = 1;
        if (bank_round_digit_p != NULL) *bank_round_digit_p = bank_round_digit;
      }
    }
  } while ((!error && !abs_equal_zero(remainder)) || one_more_iteration);
  s21_big_set_sign(big_result, result_sign);
  return overflow;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  return process_arithmetic(value_1, value_2, result, DIV);
}
