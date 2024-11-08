#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum kOpts {
    OPT_Q,
    OPT_M,
    OPT_T,
    OPT_INVALID = -1
} kOpts;

int char_int(const char *str) { // функция atoi (преобразование строки в целое число)
    int result = 0;
    int sign = 1; 
    if (*str == '-') {
        sign = -1;
        str++; 
    } else if (*str == '+') {
        str++; 
    }
    while (*str != '\0' && isdigit(*str)) {
        result = result * 10 + (*str - '0');
        str++;
    }
    return sign * result;
}

double char_double(const char *str) { // функция atof (преобразование строки в число с плавающей точкой)
    double result = 0.0;
    int sign = 1; 
    if (*str == '-') {
        sign = -1; 
        str++;
    } else if (*str == '+') {
        str++; 
    }
    while (*str != '\0' && isdigit(*str)) {
        result = result * 10 + (*str - '0');
        str++;
    }
    if (*str == '.') {
        str++;
        double decimal = 0.0;
        int power = -1;
        while (*str != '\0' && isdigit(*str)) {
            decimal = decimal * 10 + (*str - '0');
            power--;
            str++;
        }
        result += decimal * pow(10, power); 
    }
    return sign * result;
}

void quadratic_equation(double a, double b, double c, double epsilon) { // решение квадратного уравнения
    double discriminant = b * b - 4 * a * c;
    if (a == 0 && b != 0 && c != 0) {
        double x = (-c) / b;
        printf("Уравнение имеет один корень: x = %.2lf\n", x);        
    } else if (fabs(discriminant) < epsilon) {
        double x = -b / (2 * a);
        printf("Уравнение имеет один корень: x = %.2lf\n", x);
    } else if (discriminant > epsilon) {
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        printf("Уравнение имеет два корня: x1 = %.2lf, x2 = %.2lf\n", x1, x2);
    } else {
        printf("Уравнение не имеет действительных корней.\n");
    }
}

bool sides_triangle(double a, double b, double c, double epsilon) { // проверка, являются ли числа сторонами прямоугольного треугольника
    double max_side = fmax(fmax(a, b), c);
    double other_side1 = fmin(fmin(a, b), c);
    double other_side2 = (a + b + c) - max_side - other_side1;
    return fabs(max_side * max_side - other_side1 * other_side1 - other_side2 * other_side2) < epsilon;
}

bool multiple(int a, int b) { // проверка кратности чисел
    return a % b == 0;
}

int GetOpts(int argc, char** argv, kOpts *option, int *number) { // получение опции и количества аргументов
    if (argc < 2) {
        *option = OPT_INVALID;
        *number = 0;
        return 1;
    }
    if (argv[1][0] == '-' || argv[1][0] == '/') {
        if (argv[1][1] == 'q') {
            *option = OPT_Q;
            *number = argc - 1;
            return 0;
        } else if (argv[1][1] == 'm') {
            *option = OPT_M;
            *number = argc - 1;
            return 0;
        } else if (argv[1][1] == 't') {
            *option = OPT_T;
            *number = argc - 1;
            return 0;
        }
    }
    *option = OPT_INVALID;
    *number = 0;
    return 1;
}

void HandlerOptQ(int number, char** argv) { // решения квадратного уравнения с перестановками коэффициентов
    if (number != 5) {
        printf("Неверное количество аргументов для опции -q.\n");
        return;
    }
    double epsilon = char_double(argv[2]);
    double a = char_double(argv[3]);
    double b = char_double(argv[4]);
    double c = char_double(argv[5]);
    printf("Решения квадратного уравнения при всех перестановках коэффициентов:\n");
    quadratic_equation(a, b, c, epsilon);
    quadratic_equation(a, c, b, epsilon);
    quadratic_equation(b, a, c, epsilon);
    quadratic_equation(b, c, a, epsilon);
    quadratic_equation(c, a, b, epsilon);
    quadratic_equation(c, b, a, epsilon);
}

void HandlerOptM(int number, char** argv) { // кратно ли первое число второму
    if (number != 3) {
        printf("Неверное количество аргументов для опции -m.\n");
        return;
    }
    int a = char_int(argv[2]);
    int b = char_int(argv[3]);
    if (multiple(a, b)) {
        printf("Число %d кратно числу %d.\n", a, b);
    } else {
        printf("Число %d не кратно числу %d.\n", a, b);
    }
}

void HandlerOptT(int number, char** argv) { // могут ли заданные параметры являться длинами сторон прямоугольного треугольника
    if (number != 5) {
        printf("Неверное количество аргументов для опции -t.\n");
        return;
    }
    double epsilon = char_double(argv[2]);
    double a = char_double(argv[3]);
    double b = char_double(argv[4]);
    double c = char_double(argv[5]);
    if (sides_triangle(a, b, c, epsilon)) {
        printf("Числа %.2lf, %.2lf, %.2lf могут быть сторонами прямоугольного треугольника.\n", a, b, c);
    } else {
        printf("Числа %.2lf, %.2lf, %.2lf не могут быть сторонами прямоугольного треугольника.\n", a, b, c);
    }
}

int main(int argc, char** argv) {
    kOpts opt = OPT_INVALID;
    int number = 0;
    void (*handlers[3])(int, char**) = { // массив указателей на функции
        HandlerOptQ,
        HandlerOptM,
        HandlerOptT
    };
    if (GetOpts(argc, argv, &opt, &number)) {
        printf("Неверный флаг или количество аргументов.\n");
        return 1;
    }
    handlers[opt](number, argv);
    return 0;
}
