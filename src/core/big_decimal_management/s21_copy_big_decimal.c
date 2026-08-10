#include "../../s21_decimal.h"

void s21_copy_big_decimal(s21_big_decimal source,
                          s21_big_decimal *destination) {
  if (destination != NULL) {
    destination->bits[0] = source.bits[0];
    destination->bits[1] = source.bits[1];
    destination->bits[2] = source.bits[2];
    destination->bits[3] = source.bits[3];
    destination->bits[4] = source.bits[4];
    destination->bits[5] = source.bits[5];
    destination->bits[6] = source.bits[6];
  }
}