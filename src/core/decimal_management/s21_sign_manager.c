#include "../../s21_decimal.h"
#include "../s21_bit_operations.h"

void s21_set_sign(s21_decimal *dec, sign_t sign_value) {
  if (dec != NULL) {
    s21_setbit(&dec->bits[3], 31, sign_value);
  }
}

sign_t s21_get_sign(s21_decimal dec) { return s21_getbit(dec.bits[3], 31); }
