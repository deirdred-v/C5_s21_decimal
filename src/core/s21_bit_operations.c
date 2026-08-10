int s21_getbit(int num, int position) {
  int mask = 1 << position;
  return (num & mask) / mask;
}

void s21_setbit(int *num, int position, int value) {
  int mask = 1 << position;
  if (value > 0) {
    *num = *num | mask;
  } else {
    *num = *num & ~mask;
  }
}
