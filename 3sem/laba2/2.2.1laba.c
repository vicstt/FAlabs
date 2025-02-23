#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <limits.h>
#include <float.h>

long double geometric_mean(int count, ...) {
    va_list args;
    va_start(args, count);

    long double product = 1.0;
    int has_zero = 0;

    for (int i = 0; i < count; i++) {
        double num = va_arg(args, double);
        if (num < 0) {
            va_end(args);
            return NAN; 
        } else if (product > DBL_MAX / num) {
            va_end(args);
            return INFINITY; 
        } else {
            product *= num;
        }
    }
    va_end(args);

    return pow(product, 1.0 / count);
}

int main() {
    long double result = geometric_mean(4, 75.01, 0, 61.0, 104.0);

    if (isnan(result)) {
        printf("Входные данные содержат отрицательные значения.\n");
    } else if (isinf(result)) {
        printf("Переполнение при вычислении среднего геометрического.\n");
    } else {
        printf("Среднее геометрическое: %.2Lf\n", result);
    }

    return 0;
}
