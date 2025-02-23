#include <stdio.h>
#include <stdarg.h>
#include <math.h>

double horners_table(double x, int n, ...) {
    va_list args;
    va_start(args, n);

    double result = va_arg(args, double);

    for (int i = 1; i <= n; i++) {
        result = result * x + va_arg(args, double);
    }

    va_end(args);
    return result;
}

int main(){
    double x = 2.0;
    double result = horners_table(x, 3, 2.0, 3.0, 1.0, 1.0);

    printf("Значение многочлена в точке x = %.2lf %.2lf\n", x, result);

    return 0;
}
