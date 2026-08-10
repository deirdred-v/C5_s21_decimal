#include "../../s21_decimal.h"

void s21_swap_big_decimal(s21_big_decimal *big_dec1,
                          s21_big_decimal *big_dec2) {
  s21_big_decimal temp_big_dec;
  s21_copy_big_decimal(*big_dec1, &temp_big_dec);
  s21_copy_big_decimal(*big_dec2, big_dec1);
  s21_copy_big_decimal(temp_big_dec, big_dec2);
}