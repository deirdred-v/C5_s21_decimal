#include "../../s21_decimal.h"
#include "../s21_bit_operations.h"

static void s21_combined_to_single(int combined_bit_pos, int *int_pos,
                                   int *bit_pos) {
  *int_pos = combined_bit_pos / 32;
  *bit_pos = combined_bit_pos - (*int_pos) * 32;
}

void s21_big_set_num_bit(s21_big_decimal *dec, int combined_bit_pos,
                         int bit_value) {
  int int_pos;
  int bit_pos;
  s21_combined_to_single(combined_bit_pos, &int_pos, &bit_pos);
  s21_setbit(&dec->bits[int_pos], bit_pos, bit_value);
}

int s21_big_get_num_bit(s21_big_decimal dec, int combined_bit_pos) {
  int int_pos;
  int bit_pos;
  s21_combined_to_single(combined_bit_pos, &int_pos, &bit_pos);
  return s21_getbit(dec.bits[int_pos], bit_pos);
}

s21_big_decimal s21_create_simple_big_dec(int num) {
  s21_big_decimal big_dec;
  s21_clear_big_decimal(&big_dec);
  big_dec.bits[0] = num;
  return big_dec;
}
