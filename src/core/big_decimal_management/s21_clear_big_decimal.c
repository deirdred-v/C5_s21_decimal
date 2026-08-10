#include "../../s21_decimal.h"

void s21_clear_big_decimal(s21_big_decimal *dec) {
  dec->bits[0] = 0;
  dec->bits[1] = 0;
  dec->bits[2] = 0;
  dec->bits[3] = 0;
  dec->bits[4] = 0;
  dec->bits[5] = 0;
  dec->bits[6] = 0;
}