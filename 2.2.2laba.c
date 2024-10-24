#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

long double power(double base, int degree) {
    if (degree == 0) {
        return 1.0; 
   } else if (degree < 0) {
        if (base > 0){
            if ((1.0 / power(base, -degree)) < DBL_MIN) {
                return NAN;
            }
            return 1.0 / power(base, -degree);
        } if (base == 0){
            return 0.0;
        } else {
            if ((1.0 / power(-base, -degree)) < DBL_MIN) {
                return NAN;
            }
            return 1.0 / power(base, -degree);
        }
    } else if (degree % 2 == 0) {
        if (base > 0){
            double half_power = power(base, degree / 2);
            if (half_power > DBL_MAX / half_power) { 
                return INFINITY;
            }
            return half_power * half_power; 
        } if (base == 0){
            return 0.0;
        } else {
            double half_power = power(-base, degree / 2);
            if (half_power > DBL_MAX / half_power) { 
                return INFINITY;
            }
            return half_power * half_power;
        }
    } else {
        if (base > 0) {
            if (base > DBL_MAX / base) {
                return INFINITY;
            }
            return base * power(base, degree - 1);
        } if (base == 0){
            return 0.0;
        } else {
            if (-base > DBL_MAX / (-base)) {
                return INFINITY;
            }
            return base * power(base, degree - 1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Неверное количество аргументов.\n");
        return 1; 
    }

    double base = atof(argv[1]); 
    int degree = atoi(argv[2]); 

    long double result = power(base, degree);

    if (isinf(result) || isnan(result)) {
        printf("Переполнение при вычислении степени.\n");
    } else {
        printf("%.2lf в степени %d равно %.10Lf\n", base, degree, result);
    }

    return 0;
}
