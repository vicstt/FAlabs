#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>


int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int final_representation(int q, int base) {
    int value;
    while ((value = gcd(q, base)) != 1) {
        q /= value;
    }
    return q == 1;
}

int representation(double decimal, int base) {
    int numerator = 1;
    int denominator = 1;

    while (decimal != (int)decimal) {
        decimal *= 10;
        denominator *= 10;
    }
    numerator = (int)decimal;

    int divider = gcd(numerator, denominator);
    numerator /= divider;
    denominator /= divider;

    if (final_representation(denominator, base)) {
        return 1;
    } else {
        return 0;
    }
}

void check(int base, int count, ...) {
  va_list args;
  va_start(args, count);

  for (int i = 0; i < count; i++) {
    double number = va_arg(args, double);
    if (representation(number, base)) {
      printf("Дробь %.4f может быть представлена в конечном виде в системе счисления с основанием %d\n", number, base);
    } else {
      printf("Дробь %.4f не может быть представлена в конечном виде в системе счисления с основанием %d\n", number, base);
    }
  }

  va_end(args);
}

int main() {
  check(2, 4, 0.125, 0.1, 0.25, 0.6);
  return 0;
}
