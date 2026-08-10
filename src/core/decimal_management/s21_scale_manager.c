#include "../../s21_decimal.h"
#include "../s21_bit_operations.h"

int s21_set_scale(s21_decimal *dec, int num) {
  int error = 0;
  if (num < 0 || num > 28) {
    error = -1;
  } else {
    for (int i = 0; i < 8; i++) {
      s21_setbit(&dec->bits[3], i + 16, s21_getbit(num, i));
    }
  }
  return error;
}

int s21_get_scale(s21_decimal dec) {
  int result = 0;
  for (int i = 0; i < 8; i++) {
    s21_setbit(&result, i, s21_getbit(dec.bits[3], i + 16));
  }
  if (result < 0 || result > 28) result = -1;
  return result;
}