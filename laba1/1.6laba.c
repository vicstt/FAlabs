#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double integrate(double (*func)(double), double a, double b, double epsilon) {
    int n = 1; // начальное количество трапеций
    double h, integral, integral_prev;

    h = (b - a) / n; // ширина одной трапеции
    integral = h * (func(a) + func(b)) / 2;

    do {
        integral_prev = integral;
        n *= 2; 
        h = (b - a) / n; //пересчитывается ширина трапеции
        integral = 0;

        for (int i = 0; i < n; i++) {
            integral += func(a + i * h) + func(a + (i + 1) * h); //вычисляется высота трапеции (сумма значений функции в левой и правой вершинах трапеции)
        }
        integral *= h / 2;

    } while (fabs(integral - integral_prev) > epsilon); 

    return integral;
}

double func_a(double x) {
    return log(1 + x) / x;
}

double func_b(double x) {
    return exp(-x * x / 2);
}

double func_c(double x) {
    return log(1 / (1 - x));
}

double func_d(double x) {
    return pow(x, x);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    double epsilon = atof(argv[1]);

    double result_a = integrate(func_a, 0.000001, 1, epsilon);
    double result_b = integrate(func_b, 0, 1, epsilon);
    double result_c = integrate(func_c, 0, 0.999999, epsilon);
    double result_d = integrate(func_d, 0, 1, epsilon);

    printf("Интеграл a: %.4f\n", result_a);
    printf("Интеграл b: %.4f\n", result_b);
    printf("Интеграл c: %.4f\n", result_c);
    printf("Интеграл d: %.4f\n", result_d);

    return 0;
}