#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  s21_big_decimal big_dec;
  s21_big_decimal big_result;
  s21_dec2big(value, &big_dec);
  big_result = big_dec;
  if (s21_big_get_sign(big_dec) == POSITIVE) {
    s21_big_set_sign(&big_result, NEGATIVE);
  } else {
    s21_big_set_sign(&big_result, POSITIVE);
  }
  error = s21_big2dec(big_result, result);
  return error;
}
