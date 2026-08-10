#include "../../s21_decimal.h"

int s21_process_num_bits(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                         s21_big_decimal *big_result, int is_sum) {
  int error = 0;
  int remainder = 0;
  for (int i = 0; i < 192; i++) {
    int bit1 = s21_big_get_num_bit(big_dec1, i);
    int bit2 = s21_big_get_num_bit(big_dec2, i);
    int result_bit = bit1 ^ bit2 ^ remainder;
    s21_big_set_num_bit(big_result, i, result_bit);
    int condition;
    if (is_sum) {
      condition = (bit1 == 1 && bit2 == 1) || (bit1 == 1 && remainder == 1) ||
                  (bit2 == 1 && remainder == 1);
    } else {
      condition = (bit1 == 0 && bit2 == 1) ||
                  (bit1 == 1 && bit2 == 1 && remainder == 1) ||
                  (bit1 == 0 && bit2 == 0 && remainder == 1);
    }
    if (condition) {
      remainder = 1;
    } else {
      remainder = 0;
    }
    if (i == 191 && remainder == 1) error = 1;
  }
  return error;
}

int s21_process_scale(s21_big_decimal *big_dec1, s21_big_decimal *big_dec2,
                      s21_big_decimal *big_result) {
  int scale1 = s21_big_get_scale(*big_dec1);
  int scale2 = s21_big_get_scale(*big_dec2);
  int error = 0;
  s21_big_decimal temp_dec;
  s21_big_decimal *value_to_change;
  int need_scale;
  if (scale1 < scale2) {
    value_to_change = big_dec1;
    need_scale = scale2 - scale1;
    s21_big_set_scale(big_result, scale2);
  } else {
    value_to_change = big_dec2;
    need_scale = scale1 - scale2;
    s21_big_set_scale(big_result, scale1);
  }
  error = s21_pow_ten_big(*value_to_change, need_scale, &temp_dec);
  s21_copy_big_decimal(temp_dec, value_to_change);
  return error;
}

int s21_process_sign(s21_big_decimal *big_dec1, s21_big_decimal *big_dec2,
                     s21_big_decimal *big_result, sign_t sign_rules[][2]) {
  int is_sum;
  sign_t sign1 = s21_big_get_sign(*big_dec1);
  sign_t sign2 = s21_big_get_sign(*big_dec2);
  s21_big_set_sign(big_dec1, POSITIVE);
  s21_big_set_sign(big_dec2, POSITIVE);
  if (sign1 == sign_rules[0][0] && sign2 == sign_rules[0][1]) {
    s21_big_set_sign(big_result, NEGATIVE);
    is_sum = 1;
  } else if (sign1 == sign_rules[1][0] && sign2 == sign_rules[1][1]) {
    is_sum = 1;
  } else if (sign1 == sign_rules[2][0] && sign2 == sign_rules[2][1]) {
    if (s21_is_greater_big(*big_dec1, *big_dec2)) {
      s21_big_set_sign(big_result, NEGATIVE);
    }
    if (s21_is_greater_big(*big_dec2, *big_dec1)) {
      s21_swap_big_decimal(big_dec1, big_dec2);
    }
    is_sum = 0;
  } else if (sign1 == sign_rules[3][0] && sign2 == sign_rules[3][1]) {
    if (s21_is_greater_big(*big_dec2, *big_dec1)) {
      s21_big_set_sign(big_result, NEGATIVE);
      s21_swap_big_decimal(big_dec1, big_dec2);
    }
    is_sum = 0;
  }
  return is_sum;
}

int s21_big_add(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                s21_big_decimal *big_result) {
  s21_clear_big_decimal(big_result);
  sign_t sign_rules[4][2] = {{NEGATIVE, NEGATIVE},
                             {POSITIVE, POSITIVE},
                             {NEGATIVE, POSITIVE},
                             {POSITIVE, NEGATIVE}};
  int is_sum = s21_process_sign(&big_dec1, &big_dec2, big_result, sign_rules);
  int error = s21_process_scale(&big_dec1, &big_dec2, big_result);
  if (!error)
    error = s21_process_num_bits(big_dec1, big_dec2, big_result, is_sum);
  return error;
}

int s21_big_sub(s21_big_decimal big_dec1, s21_big_decimal big_dec2,
                s21_big_decimal *big_result) {
  s21_clear_big_decimal(big_result);
  sign_t sign_rules[4][2] = {{NEGATIVE, POSITIVE},
                             {POSITIVE, NEGATIVE},
                             {NEGATIVE, NEGATIVE},
                             {POSITIVE, POSITIVE}};
  int is_sum = s21_process_sign(&big_dec1, &big_dec2, big_result, sign_rules);
  int error = s21_process_scale(&big_dec1, &big_dec2, big_result);
  if (!error)
    error = s21_process_num_bits(big_dec1, big_dec2, big_result, is_sum);
  return error;
}

int process_arithmetic(s21_decimal dec1, s21_decimal dec2, s21_decimal *result,
                       operation_t operation_type) {
  int error = 0;
  if (s21_get_scale(dec1) == -1 || s21_get_scale(dec2) == -1 || result == NULL)
    error = 1;
  if (!error) {
    s21_big_decimal big_dec1;
    s21_big_decimal big_dec2;
    s21_big_decimal big_result;
    s21_dec2big(dec1, &big_dec1);
    s21_dec2big(dec2, &big_dec2);

    if (operation_type == DIV && abs_equal_zero(big_dec2)) error = 3;
    int overflow = 0;
    int round_digit;
    if (!error) {
      switch (operation_type) {
        case ADD:
          s21_big_add(big_dec1, big_dec2, &big_result);
          break;
        case SUB:
          s21_big_sub(big_dec1, big_dec2, &big_result);
          break;
        case MUL:
          s21_big_mul(big_dec1, big_dec2, &big_result);
          break;
        case DIV:
          overflow = s21_big_div(big_dec1, big_dec2, &big_result, &round_digit,
                                 NULL, FULL_DIV);
          break;
      }
      int convert_err = s21_big2dec(big_result, result);
      if (convert_err || overflow) {
        s21_big_decimal temp_big_dec;
        if (operation_type == DIV) {
          s21_copy_big_decimal(big_result, &temp_big_dec);
          s21_apply_div_round_rules(&temp_big_dec, round_digit);
          if (abs_equal_zero(temp_big_dec)) error = 2;
        } else {
          s21_bank_round(big_result, &temp_big_dec);
        }
        if (!error) error = s21_big2dec(temp_big_dec, result);
        if (error == 1 && s21_big_get_sign(big_result) == NEGATIVE) error = 2;
      }
    }
    if (error) {
      s21_clear_decimal(result);
      sign_t zero_sign = process_mul_sign(&big_dec1, &big_dec2);
      s21_set_sign(result, zero_sign);
    }
  }

  return error;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  return process_arithmetic(value_1, value_2, result, ADD);
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  return process_arithmetic(value_1, value_2, result, SUB);
}