#include "../../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  int scale = s21_get_scale(src);

  if (dst == NULL || scale < 0 || scale > 28) {
    error = 1;
  } else if ((src.bits[3] & 0xFFFF) != 0 || (src.bits[3] & 0x7F000000) != 0) {
    error = 1;
  } else {
    if (scale > 0) {
      s21_truncate(src, &src);
    }
    error = s21_dec2int(src, dst);
  }
  return error;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;
  } else {
    s21_clear_decimal(dst);
    if (src < 0) {
      dst->bits[0] = (unsigned int)(-(long long)src);
      s21_set_sign(dst, 1);
    } else {
      dst->bits[0] = src;
    }
  }
  return error;
}