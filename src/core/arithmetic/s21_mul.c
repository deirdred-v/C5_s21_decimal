#include "../../s21_decimal.h"

int s21_shift_bits_left_big(s21_big_decimal *dec, int shift) {
  int error = 0;
  if (shift < 0) error = 1;
  for (int i = 191; i >= 0; i--) {
    if (i >= 192 - shift && s21_big_get_num_bit(*dec, i) == 1) error = 1;
    if (!error) {
      if (i - shift >= 0) {
        s21_big_set_num_bit(dec, i, s21_big_get_num_bit(*dec, i - shift));
      } else {
        s21_big_set_num_bit(dec, i, 0);
      }
    }
  }
  return error;
}

sign_t process_mul_sign(s21_big_decimal *big_dec1, s21_big_decimal *big_dec2) {
  sign_t result = POSITIVE;
  sign_t sign1 = s21_big_get_sign(*big_dec1);
  sign_t sign2 = s21_big_get_sign(*big_dec2);
  if (sign1 == NEGATIVE || sign2 == NEGATIVE) result = NEGATIVE;
  if (sign1 == NEGATIVE && sign2 == NEGATIVE) result = POSITIVE;
  s21_big_set_sign(big_dec1, POSITIVE);
  s21_big_set_sign(big_dec2, POSITIVE);
  return result;
}

int s21_big_mul(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                s21_big_decimal *big_result) {
  int error = 0;
  s21_clear_big_decimal(big_result);
  s21_big_decimal temp_sum;
  int shift = 0;
  for (int i = 0; i < 192; i++) {
    if (s21_big_get_num_bit(big_dec2, i) == 1 && !error) {
      error = s21_shift_bits_left_big(&big_dec1, shift);
      shift = 0;
      if (!error) error = s21_big_add(big_dec1, *big_result, &temp_sum);
      s21_copy_big_decimal(temp_sum, big_result);
    }
    shift++;
  }
  int scale_result = s21_big_get_scale(big_dec1) + s21_big_get_scale(big_dec2);
  s21_big_set_scale(big_result, scale_result);
  sign_t result_sign = process_mul_sign(&big_dec1, &big_dec2);
  s21_big_set_sign(big_result, result_sign);
  return error;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  return process_arithmetic(value_1, value_2, result, MUL);
}
