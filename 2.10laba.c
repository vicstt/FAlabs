#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

int factorial(int num) {
    if (num == 0 || num == 1) return 1;
    int result = 1;
    for (int i = 2; i <= num; i++) {
        result *= i;
    }
    return result;
}

double evaluate_polynomial(double a, int n, double* coeffs) {
    double result = 0.0;
    for (int i = 0; i <= n; i++) {
        result += coeffs[i] * pow(a, i);
    }
    return result;
}

double derivative(double a, int n, double* coeffs, int order) {
    double result = 0.0;
    for (int i = order; i <= n; i++) {
        double coeff = coeffs[i];
        for (int j = 0; j < order; j++) {
            coeff *= (i - j); 
        }
        result += coeff * pow(a, i - order);
    }
    return result;
}

void polynomial_decomposition(double epsilon, double a, double** g_coeffs, int n, ...) {
    *g_coeffs = (double*)malloc((n + 1) * sizeof(double));
    if (*g_coeffs == NULL) {
        printf("Ошибка выделения памяти.\n");
        return;
    }

    va_list args;
    va_start(args, n);

    double* f_coeffs = (double*)malloc((n + 1) * sizeof(double));
    for (int i = 0; i <= n; i++) {
        f_coeffs[i] = va_arg(args, double);
    }
    va_end(args);

    for (int i = 0; i <= n; i++) {
        (*g_coeffs)[i] = derivative(a, n, f_coeffs, i) / factorial(i);
    }

    free(f_coeffs);
}

int main() {
    double* g_coeffs;
    double a = 1.0; 
    double epsilon = 0.0001; 
    int n = 2; 

    polynomial_decomposition(epsilon, a, &g_coeffs, n, 1.0, -3.0, 2.0);

    printf("Коэффициенты g(x):\n");
    for (int i = 0; i <= n; i++) {
        printf("g[%d] = %.5f\n", i, g_coeffs[i]);
    }

    free(g_coeffs);
    return 0;
}
