#include "../../s21_decimal.h"
#include "../s21_bit_operations.h"

void s21_big_set_sign(s21_big_decimal *dec, sign_t sign_value) {
  s21_setbit(&dec->bits[6], 31, sign_value);
}

sign_t s21_big_get_sign(s21_big_decimal dec) {
  return s21_getbit(dec.bits[6], 31);
}
