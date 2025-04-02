#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f1(double x) {
    return x * x - 4; 
}

double f2(double x) {
    return x * x * x - x; 
}

double f3(double x) {
    if (fabs(x - 1) < 0.1) {
        return NAN; 
    }
    return pow(x - 1, 2) / (x - 1);
}

double f4(double x) {
    return pow(x - 1, 3); 
}

int has_discontinuity(double x, double (*func)(double)) {
    return isnan(func(x));
}

double dichotomy(double a, double b, double epsilon, double (*func)(double)) {
    if (has_discontinuity(a, func) || has_discontinuity(b, func)) {
        printf("Функция имеет точку разрыва на границах интервала [%.2f, %.2f].\n", a, b);
        return -1;
    }

    double mid;
    mid = (a + b) / 2.0;

    if (has_discontinuity(mid, func)) {
        printf("Функция имеет точку разрыва в середине интервала [%.2f, %.2f].\n", a, b);
        return -1;
    }

    if (func(a) * func(b) >= 0) {
        printf("Корень на данном интервале не найден.\n");
        return -1;
    }

    while (fabs(func(mid)) > epsilon) {
        if (func(a) * func(mid) < 0) {
            b = mid;
        } else {
            a = mid;
        }
        mid = (a + b) / 2.0;

        if (has_discontinuity(mid, func)) {
            printf("Функция имеет точку разрыва в середине интервала [%.2f, %.2f].\n", a, b);
            return -1;
        }
    }
    return mid;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    double epsilon = atof(argv[3]);
    int func_index = atoi(argv[4]);

    double (*func)(double) = NULL;

    switch(func_index) {
        case 1:
            func = f1;
            break;
        case 2:
            func = f2;
            break;
        case 3:
            func = f3;
            break;
        case 4:
            func = f4;
            break;
        default:
            printf("Недопустимая функция. Используйте 1, 2, 3 или 4.\n");
            return 1;
    }

    double root = dichotomy(a, b, epsilon, func);
    if (root != -1) {
        printf("Найденный корень: %.2f\n", root);
    }
    return 0;
}
