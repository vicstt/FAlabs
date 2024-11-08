#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double factorial(int n) {
    if (n == 0 || n == 1) return 1.0;
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double double_factorial(int n) {
    if (n <= 0) return 1.0;
    double result = 1.0;
    for (int i = n; i > 0; i -= 2) {
        result *= i;
    }
    return result;
}

double sum_a(double x, double epsilon) {
    double sum = 0.0;
    double term;
    int n = 0;
    do {
        term = pow(x, n) / factorial(n);
        sum += term;
        n++;
    } while (fabs(term) >= epsilon);
    return sum;
}

double sum_b(double x, double epsilon) {
    double sum = 0.0;
    double term;
    int n = 0;
    do {
        term = pow(-1, n) * pow(x, 2 * n) / factorial(2 * n);
        sum += term;
        n++;
    } while (fabs(term) >= epsilon);
    return sum;
}

double sum_c(double x, double epsilon) {
    double sum = 0.0;
    double term;
    int n = 0;
    do {
        term = (pow(3, 3 * n) * pow(factorial(n), 3) * pow(x, 2 * n)) / factorial(3 * n);
        sum += term;
        n++;
    } while (fabs(term) >= epsilon);
    return sum;
}

double sum_d(double x, double epsilon) {
    double sum = 0.0;
    double term;
    int n = 1;
    do {
        term = pow(-1, n) * double_factorial(2 * n - 1) * pow(x, 2 * n) / double_factorial(2 * n);
        sum += term;
        n++;
    } while (fabs(term) >= epsilon);
    return sum;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    double epsilon = atof(argv[1]);
    double x = atof(argv[2]);

    printf("Сумма a: %.4lf\n", sum_a(x, epsilon));
    if (isnan(sum_a(x, epsilon))){
        printf("Сумма расходится.\n");
    }
    printf("Сумма b: %.4lf\n", sum_b(x, epsilon));
    if (isnan(sum_b(x, epsilon))){
        printf("Сумма расходится.\n");
    }
    printf("Сумма c: %.4lf\n", sum_c(x, epsilon));
    if (isnan(sum_c(x, epsilon))){
        printf("Сумма расходится.\n");
    }
    printf("Сумма d: %.4lf\n", sum_d(x, epsilon));
    if (isnan(sum_d(x, epsilon))){
        printf("Сумма расходится.\n");
    }
    return 0;
}
