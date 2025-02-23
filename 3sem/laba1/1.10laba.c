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

int to_decimal(const char *number, int system) { //преобразование числа из заданной системы счисления в десятичную
    int len = strlen(number);
    int degree = strlen(number) - 1;
    int decimal = 0;
    for (int i = 0; i < len; i++) {
        int digit = char_int(number[i]);
        if (digit < 0 || digit >= system) {
            return -1; 
        }
        decimal += digit * pow(system, degree);
        degree--;
    }
    return decimal;
}

char *to_base(int decimal, int base) { //преобразование числа из десятичной системы счисления в заданную
    char *result = malloc(64); 
    if (result == NULL) {
        return NULL;
    }
    int i = 0;
    do {
        int digit = decimal % base;
        if (digit < 10) {
            result[i++] = digit + '0';
        } else {
            result[i++] = digit - 10 + 'A';
        }
        decimal /= base;
    } while (decimal > 0);
    result[i] = '\0';
    int len = strlen(result);
    for (int i = 0; i < len / 2; i++) {
        char temp = result[i];
        result[i] = result[len - i - 1];
        result[len - i - 1] = temp;
    }
    return result;
}

int main() {
    int base;
    char input[100];
    printf("Введите основание системы счисления (2-36): ");
    scanf("%d", &base);

    if (base < 2 || base > 36) {
        printf("Некорректное основание системы счисления.\n");
        return 1;
    }

    char number[100];
    long long max_number = 0;
    char *max_number_str = NULL;

    printf("Введите числа в системе счисления с основанием %d (введите Stop для завершения):\n", base);

    while (strcmp(number, "Stop") != 0) {
        scanf("%s", number);
        int decimal = to_decimal(number, base);
        if (decimal == -1) {
            continue; 
        }
        if (abs(decimal) > abs(max_number)) {
            max_number = decimal;
            if (max_number_str) {
                free(max_number_str);
            }
            max_number_str = to_base(max_number, base);
        }
    }

    if (max_number_str == NULL) {
        printf("Не было введено ни одного корректного числа.\n");
        return 1;
    }

    printf("Максимальное по модулю число: %s\n", max_number_str);
    printf("Строковые представления в разных системах счисления:\n");
    printf("Основание 9: %s\n", to_base(max_number, 9));
    printf("Основание 18: %s\n", to_base(max_number, 18));
    printf("Основание 27: %s\n", to_base(max_number, 27));
    printf("Основание 36: %s\n", to_base(max_number, 36));

    free(max_number_str);

    return 0;
}
