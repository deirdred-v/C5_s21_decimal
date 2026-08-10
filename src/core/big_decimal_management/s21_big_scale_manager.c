#include "../../s21_decimal.h"
#include "../s21_bit_operations.h"

void s21_big_set_scale(s21_big_decimal *dec, int num) {
  for (int i = 0; i < 8; i++) {
    s21_setbit(&dec->bits[6], i + 16, s21_getbit(num, i));
  }
}

int s21_big_get_scale(s21_big_decimal dec) {
  int result = 0;
  for (int i = 0; i < 8; i++) {
    s21_setbit(&result, i, s21_getbit(dec.bits[6], i + 16));
  }
  return result;
}