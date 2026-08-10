#include "../../s21_decimal.h"

uint32_t div_96_by_10(uint32_t bits[3]) {
  uint64_t rem = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t cur = (rem << 32) | bits[i];
    bits[i] = (uint32_t)(cur / 10);
    rem = cur % 10;
  }
  return (uint32_t)rem;
}

int validate_float_for_conversion(float src, s21_decimal *dst) {
  int error = 0;
  if (dst == NULL || isnan(src) || isinf(src)) {
    error = 1;
  } else if (fabsf(src) > 7.9228162514264337593543950335e+28f) {
    error = 1;
  }
  return error;
}

void process_float_value(double *d, int *scale) {
  while (*scale < 28 && *d < 1.0) {
    *d *= 10.0;
    (*scale)++;
  }
  while (*scale < 28 && fabs(floor(*d) - *d) > 1e-10) {
    *d *= 10.0;
    (*scale)++;
  }
  *d = nearbyint(*d);
}

int convert_float_to_bits_and_validate(double d, uint32_t bits[3], int *scale) {
  int error = 0;
  if (d > 7.9228162514264337593543950335e+28) {
    error = 1;
  } else {
    double tmp = d;
    for (int i = 0; i < 3; i++) {
      bits[i] = (uint32_t)fmod(tmp, 4294967296.0);
      tmp = floor(tmp / 4294967296.0);
    }
    while (*scale > 0) {
      uint32_t bits_copy[3] = {bits[0], bits[1], bits[2]};
      if (div_96_by_10(bits_copy) != 0) {
        break;
      }
      bits[0] = bits_copy[0];
      bits[1] = bits_copy[1];
      bits[2] = bits_copy[2];
      (*scale)--;
    }
    if (bits[2] != 0) {
      error = 1;
    }
  }
  return error;
}

void set_decimal_result(s21_decimal *dst, uint32_t bits[3], int scale,
                        int sign) {
  dst->bits[0] = bits[0];
  dst->bits[1] = bits[1];
  dst->bits[2] = bits[2];
  s21_set_scale(dst, scale);
  s21_set_sign(dst, sign);
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = validate_float_for_conversion(src, dst);
  if (error == 0) {
    if (src == 0.0f) {
      s21_clear_decimal(dst);
      if (signbit(src)) {
        s21_set_sign(dst, 1);
      }
    } else {
      int sign = (src < 0.0f || signbit(src)) ? 1 : 0;
      double d = fabs((double)src);
      int scale = 0;
      process_float_value(&d, &scale);
      uint32_t bits[3] = {0, 0, 0};
      error = convert_float_to_bits_and_validate(d, bits, &scale);
      if (error == 0) {
        set_decimal_result(dst, bits, scale, sign);
      }
    }
  }
  return error;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;
  } else {
    int scale = s21_get_scale(src);
    if (scale < 0 || scale > 28) {
      error = 1;
    } else if ((src.bits[3] & 0xFFFF) != 0 || (src.bits[3] & 0x7F000000) != 0) {
      error = 1;
    } else {
      double value = (double)src.bits[0] + (double)src.bits[1] * 4294967296.0 +
                     (double)src.bits[2] * 18446744073709551616.0;
      for (int i = 0; i < scale; i++) {
        value /= 10.0;
      }
      if (s21_get_sign(src)) {
        value = -value;
      }
      *dst = (float)value;
    }
  }
  return error;
}