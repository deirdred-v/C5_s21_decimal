#include "../../s21_decimal.h"

void s21_apply_bank_round_rules(s21_big_decimal *big_dec, int round_digit,
                                int bank_round_digit) {
  s21_big_decimal one_big_dec = s21_create_simple_big_dec(1);
  s21_big_set_scale(&one_big_dec, s21_big_get_scale(*big_dec));
  s21_big_set_sign(&one_big_dec, s21_big_get_sign(*big_dec));
  if (round_digit > 5 || (round_digit == 5 && bank_round_digit % 2 == 1))
    s21_big_add(*big_dec, one_big_dec, big_dec);
}

void s21_apply_div_round_rules(s21_big_decimal *big_dec, int round_digit) {
  s21_big_decimal one_big_dec = s21_create_simple_big_dec(1);
  s21_big_set_scale(&one_big_dec, s21_big_get_scale(*big_dec));
  s21_big_set_sign(&one_big_dec, s21_big_get_sign(*big_dec));
  if (round_digit >= 5) s21_big_add(*big_dec, one_big_dec, big_dec);
}

void s21_apply_round_rules(s21_big_decimal *big_dec, int round_digit) {
  s21_big_decimal one_big_dec = s21_create_simple_big_dec(1);
  s21_big_set_sign(&one_big_dec, s21_big_get_sign(*big_dec));
  if (round_digit >= 5) s21_big_add(*big_dec, one_big_dec, big_dec);
}

void s21_apply_floor_rules(s21_big_decimal *big_dec, int round_digit) {
  s21_big_decimal one_big_dec = s21_create_simple_big_dec(1);
  s21_big_set_sign(&one_big_dec, NEGATIVE);
  if (round_digit != 0 && s21_big_get_sign(*big_dec) == NEGATIVE)
    s21_big_add(*big_dec, one_big_dec, big_dec);
}