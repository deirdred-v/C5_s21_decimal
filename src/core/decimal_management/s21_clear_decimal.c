#include "../../s21_decimal.h"

void s21_clear_decimal(s21_decimal *dec) {
  if (dec != NULL) {
    dec->bits[0] = 0;
    dec->bits[1] = 0;
    dec->bits[2] = 0;
    dec->bits[3] = 0;
  }
}