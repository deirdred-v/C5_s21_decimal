#include "../s21_decimal.h"

int s21_fit_in_decimal(s21_big_decimal big_dec) {
  int is_fit = 1;
  if (big_dec.bits[3] != 0 || big_dec.bits[4] != 0 || big_dec.bits[5] != 0)
    is_fit = 0;
  if (s21_big_get_scale(big_dec) > 28) is_fit = 0;
  return is_fit;
}

int s21_fit_in_int(s21_decimal dec) {
  int is_fit = 1;
  if (dec.bits[1] != 0 || dec.bits[2] != 0) is_fit = 0;
  return is_fit;
}

void s21_dec2big(s21_decimal source, s21_big_decimal *destination) {
  if (destination != NULL) {
    destination->bits[0] = source.bits[0];
    destination->bits[1] = source.bits[1];
    destination->bits[2] = source.bits[2];
    destination->bits[3] = 0;
    destination->bits[4] = 0;
    destination->bits[5] = 0;
    destination->bits[6] = source.bits[3];
  }
}

int s21_big2dec(s21_big_decimal source, s21_decimal *destination) {
  int error = 0;
  if (destination == NULL) {
    error = 1;
  } else if (s21_fit_in_decimal(source)) {
    destination->bits[0] = source.bits[0];
    destination->bits[1] = source.bits[1];
    destination->bits[2] = source.bits[2];
    destination->bits[3] = source.bits[6];
  } else {
    error = 1;
  }

  return error;
}

int s21_dec2int(s21_decimal source, int *destination) {
  int error = 0;
  if (destination == NULL) {
    error = 1;
  } else if (s21_fit_in_int(source)) {
    int sign = s21_get_sign(source);
    unsigned int abs_val = source.bits[0];
    if ((!sign && abs_val > 2147483647u) || (sign && abs_val > 2147483648u)) {
      error = 1;
    } else {
      *destination = (int)abs_val;
      if (sign) {
        *destination = -*destination;
      }
    }
  } else {
    error = 1;
  }
  return error;
}

void s21_big2int(s21_big_decimal source, int *destination) {
  if (destination != NULL) {
    *destination = source.bits[0] % 10;
  }
}