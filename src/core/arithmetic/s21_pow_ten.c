#include "../../s21_decimal.h"

int s21_pow_ten_big(s21_big_decimal big_dec, int power,
                    s21_big_decimal *big_result) {
  int error = 0;
  s21_clear_big_decimal(big_result);
  if (power == 0) s21_copy_big_decimal(big_dec, big_result);

  s21_big_decimal ten_big_dec = s21_create_simple_big_dec(10);
  while (power && !error) {
    error = s21_big_mul(big_dec, ten_big_dec, big_result);
    s21_copy_big_decimal(*big_result, &big_dec);
    power--;
  }
  return error;
}
