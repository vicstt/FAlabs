#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int char_int(char num) { // преобразование числа из символьного типа данных в целый
    if (isdigit(num)) {
        return num - '0';
    } else if (isupper(num)) {
        return num - 'A' + 10;
    } else if (islower(num)) {
        return num - 'a' + 10;
    } else {
        return -1; 
    }
}

int number_system(const char *number) { // определение минимального основания системы счисления
    int len = strlen(number);
    int max_digit = 0;
    for (int i = 0; i < len; i++) {
        int digit = char_int(number[i]);
        if (digit > max_digit) {
            max_digit = digit;
        }
    }
    return max_digit + 1; 
}

int to_decimal(const char *number, int system) { //преобразование числа из заданной системы счисления в десятичную
    int len = strlen(number);
    int stepen = strlen(number) - 1;
    int decimal = 0;
    for (int i = 0; i < len; i++) {
        int digit = char_int(number[i]);
        if (digit < 0 || digit >= system) {
            return -1; 
        }
        decimal += digit * pow(system, stepen);
        stepen--;
    }
    return decimal;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Не удалось открыть файл %s\n", argv[1]);
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("Не удалось открыть файл %s\n", argv[2]);
        return 1;
    }

    char number[100];
    while (fscanf(input, "%s", number) != EOF) {
        int system = number_system(number);
        int decimal = to_decimal(number, system);
        if (decimal == -1) {
            printf("Некорректное число: %s\n", number);
            continue;
        }
        fprintf(output, "%s %d %d\n", number, system, decimal);
    }

    fclose(input);
    fclose(output);

    return 0;
}