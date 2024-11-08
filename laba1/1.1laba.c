#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_E,
    OPT_A,
    OPT_F,
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

int GetOpts(int argc, char** argv, kOpts *option, int *x) { // извлекает флаг, количество аргументов и преобразует строку в число x.
    if (argc < 3) {
        *option = OPT_INVALID;
        return 1;
    }
    *x = char_int(argv[2]);
    if (argv[1][0] == '-' || argv[1][0] == '/') {
        if (argv[1][1] == 'h') {
            *option = OPT_H;
            return 0;
        } else if (argv[1][1] == 'p') {
            *option = OPT_P;
            return 0;
        } else if (argv[1][1] == 's') {
            *option = OPT_S;
            return 0;
        } else if (argv[1][1] == 'e') {
            *option = OPT_E;
            return 0;
        } else if (argv[1][1] == 'a') {
            *option = OPT_A;
            return 0;
        } else if (argv[1][1] == 'f') {
            *option = OPT_F;
            return 0;
        }
    }
    *option = OPT_INVALID;
    return 1;
}

void HandlerOptH(int x) { // натуральные числа в пределах 100, кратные х
        if (x <= 0) {
            printf("Число должно быть натуральным.\n");
            return;
        }
        if (x > 100) {
            printf("Нет кратных чисел.\n");
        }
        else {
            printf("Кратные %d числа в пределах 100:\n", x);
            for (int i = 1; i <= 100; i++) {
                if (i % x == 0) {
                    printf("%d ", i);
                }
            }
            printf("\n");
        }
}

void HandlerOptP(int x) { // является ли х простым и составным
    if (x <= 1) {
        printf("%d не является ни простым, ни составным.\n", x);
        return;
    }
    bool isPrime = true;
    for (int i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            isPrime = false;
            break;
        }
    }
    if (isPrime) {
        printf("%d - простое число.\n", x);
    } else {
        printf("%d - составное число.\n", x);
    }
}

void HandlerOptS(int x) { // вывести цифры х сс с основанием 16
    int ost;
    char digits[20];
    int i = 0;
    printf("Число %d в шестнадцатеричной системе:\n", x);
    if (x == 0){
        printf("0 \n");
        return;
    }
    if (x < 0) {
        printf("-");
        x = - x;
    }
    while (x != 0) {
        int ost = x % 16;
        char hexdig;
        if (ost < 10) {
            hexdig = ost + '0';
        } else {
            hexdig = ost - 10 + 'A';
        }
        digits[i++] = hexdig;
        x = x / 16;
    }
    for (int j = i - 1; j >= 0; j--) {
        printf("%c ", digits[j]);
    }
    printf("\n");
}

void HandlerOptE(int x) { // таблица степеней
    if (x > 10 || x < 1){
            printf("Число должно быть не больше 10 и не меньше 1.\n");
            return;
    }
    printf("Таблица степеней:\n");
    for (int a = 1; a <= 10; a++){
        for (int b = 1; b <= x; b++){ 
            long long res = 1;
            for (int i = 0; i < b; i++){ 
                res *= a; 
            }
            printf("%d ^ %d = %lld\n", a, b, res);
        }
        printf("\n"); 
    }
}

void HandlerOptA(int x) { // сумма чисео от 1 до х
    long long sum = 0;
    if (x <= 0) {
            printf("Число должно быть не меньше единицы.\n");
            return;
    }
    if (x >= 46341) {
            printf("Число должно быть не больше 46340.\n");
            return;
    }
    sum = ((1 + x) * x) / 2;
    printf("Сумма натуральных чисел от 1 до %d: %lld\n", x, sum);
}

void HandlerOptF(int x) { // факториал х
    if (x < 0) {
        printf("Число должно быть неотрицательным.\n");
        return;
    }
    if (x >= 20) {
        printf("Число должно быть меньше 26.\n");
        return;
    }
    long long factorial = 1;
    for (int i = 1; i <= x; i++) {
        factorial *= i;
    }
    printf("Факториал %d: %lld\n", x, factorial);
}

int main(int argc, char** argv) {
    kOpts opt = OPT_INVALID;
    int x = 0;
    void (*handlers[6])(int) = { // массив указателей на функции
        HandlerOptH,
        HandlerOptP,
        HandlerOptS,
        HandlerOptE,
        HandlerOptA,
        HandlerOptF
    };
    if (GetOpts(argc, argv, &opt, &x)) {
        printf("Неверный флаг или количество аргументов.\n");
        return 1;
    }
    handlers[opt](x); 
    return 0;
}
