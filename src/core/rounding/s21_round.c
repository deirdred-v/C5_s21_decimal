#include "../../s21_decimal.h"

void s21_process_round(s21_big_decimal big_dec, s21_big_decimal *big_result,
                       round_t round_type) {
  int scale = s21_big_get_scale(big_dec);
  s21_big_set_scale(&big_dec, 0);
  s21_big_decimal one_big_dec = s21_create_simple_big_dec(1);
  s21_pow_ten_big(one_big_dec, scale, &one_big_dec);

  int round_digit = 0;
  int bank_round_digit = 0;
  if (round_type == BANK_ROUND) {
    s21_big_div(big_dec, one_big_dec, big_result, &round_digit,
                &bank_round_digit, FULL_DIV);
  } else {
    s21_big_div(big_dec, one_big_dec, big_result, &round_digit, NULL, INT_DIV);
  }

  switch (round_type) {
    case BANK_ROUND:
      s21_apply_bank_round_rules(big_result, round_digit, bank_round_digit);
      break;
    case ROUND:
      s21_apply_round_rules(big_result, round_digit);
      break;
    case FLOOR:
      s21_apply_floor_rules(big_result, round_digit);
      break;
    case TRUNCATE:
      break;
  }
}

void s21_bank_round(s21_big_decimal big_dec, s21_big_decimal *big_result) {
  s21_process_round(big_dec, big_result, BANK_ROUND);
}

int s21_truncate(s21_decimal dec, s21_decimal *result) {
  s21_big_decimal big_dec;
  s21_big_decimal big_result;
  s21_dec2big(dec, &big_dec);
  s21_process_round(big_dec, &big_result, TRUNCATE);
  int convert_err = s21_big2dec(big_result, result);
  return convert_err;
}

int s21_floor(s21_decimal dec, s21_decimal *result) {
  s21_big_decimal big_dec;
  s21_big_decimal big_result;
  s21_dec2big(dec, &big_dec);
  s21_process_round(big_dec, &big_result, FLOOR);
  int convert_err = s21_big2dec(big_result, result);
  return convert_err;
}

int s21_round(s21_decimal dec, s21_decimal *result) {
  s21_big_decimal big_dec;
  s21_big_decimal big_result;
  s21_dec2big(dec, &big_dec);
  s21_process_round(big_dec, &big_result, ROUND);
  int convert_err = s21_big2dec(big_result, result);
  return convert_err;
}