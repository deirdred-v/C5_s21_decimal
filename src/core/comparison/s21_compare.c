#include "../../s21_decimal.h"

int abs_equal_zero(s21_big_decimal big_dec) {
  int is_zero = 1;
  for (int i = 0; i < 6; i++) {
    if (big_dec.bits[i] != 0) is_zero = 0;
  }
  return is_zero;
}

int abs_zero_compare(s21_big_decimal big_dec1, s21_big_decimal big_dec2) {
  int is_zero1 = abs_equal_zero(big_dec1);
  int is_zero2 = abs_equal_zero(big_dec2);
  return is_zero1 && is_zero2;
}

int s21_compare_big(s21_big_decimal big_dec1, s21_big_decimal big_dec2) {
  int result = EQUAL;

  s21_big_decimal temp_result;
  s21_process_scale(&big_dec1, &big_dec2, &temp_result);

  for (int i = 191; i >= 0 && result == EQUAL; i--) {
    int bit1 = s21_big_get_num_bit(big_dec1, i);
    int bit2 = s21_big_get_num_bit(big_dec2, i);
    if (bit1 > bit2) {
      result = GREATER;
    } else if (bit1 < bit2) {
      result = LESS;
    }
  }

  sign_t sign1 = s21_big_get_sign(big_dec1);
  sign_t sign2 = s21_big_get_sign(big_dec2);
  if (sign1 == NEGATIVE && sign2 == NEGATIVE) {
    result *= -1;
  } else {
    if (result == GREATER && sign1 == NEGATIVE) result = LESS;
    if (result == LESS && sign2 == NEGATIVE) result = GREATER;
    if (result == EQUAL && sign1 > sign2 &&
        !abs_zero_compare(big_dec1, big_dec2))
      result = LESS;
    if (result == EQUAL && sign1 < sign2 &&
        !abs_zero_compare(big_dec1, big_dec2))
      result = GREATER;
  }
  return result;
}

int s21_is_greater_big(s21_big_decimal big_value_1,
                       s21_big_decimal big_value_2) {
  int compare = s21_compare_big(big_value_1, big_value_2);
  return compare == GREATER;
}

int s21_compare(s21_decimal dec1, s21_decimal dec2) {
  s21_big_decimal big_dec1;
  s21_big_decimal big_dec2;
  s21_dec2big(dec1, &big_dec1);
  s21_dec2big(dec2, &big_dec2);
  int compare = s21_compare_big(big_dec1, big_dec2);
  return compare;
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  int compare = s21_compare(dec1, dec2);
  return compare == GREATER;
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  int compare = s21_compare(dec1, dec2);
  return (compare == GREATER || compare == EQUAL);
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  int compare = s21_compare(dec1, dec2);
  return compare == LESS;
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  int compare = s21_compare(dec1, dec2);
  return (compare == LESS || compare == EQUAL);
}

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int compare = s21_compare(dec1, dec2);
  return compare == EQUAL;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  int compare = s21_compare(dec1, dec2);
  return compare != EQUAL;
}