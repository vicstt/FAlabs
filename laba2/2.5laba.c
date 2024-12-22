#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

char* int_roman(int num) {
    if (num < 1 || num > 3999) {
        return NULL;
    }

    const char* roman_chars[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const int roman_values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};

    char* result = (char*)malloc(16 * sizeof(char)); 
    if (result == NULL) {
        printf("Ошибка выделения памяти.\n");
        return NULL;
    }

    result[0] = '\0';

    for (int i = 0; i < 13; i++) {
        while (num >= roman_values[i]) {
            strcat(result, roman_chars[i]);
            num -= roman_values[i];
        }
    }

    return result;
}

char* int_zeckendorf(unsigned int num) {
    if (num == 0) {
        char* result = (char*)malloc(2 * sizeof(char));
        if (result == NULL) {
            printf("Ошибка выделения памяти.\n");
            return NULL;
        }
        strcpy(result, "0");
        return result;
    }

    int fib[32];
    fib[0] = 1;
    fib[1] = 2;
    int i = 1;
    while (fib[i] <= num) {
        i++;
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    char* result = (char*)malloc((i + 2) * sizeof(char)); 
    if (result == NULL) {
        printf("Ошибка выделения памяти.\n");
        return NULL;
    }

    result[0] = '\0';

    for (int j = i - 1; j >= 0; j--) {
        if (num >= fib[j]) {
            strcat(result, "1");
            num -= fib[j];
        } else {
            strcat(result, "0");
        }
    }

    int len = strlen(result);
    for (int i = 0; i < len / 2; i++) {
        char temp = result[i];
        result[i] = result[len - 1 - i];
        result[len - 1 - i] = temp;
    }

    strcat(result, "1");
    return result;
}

char* int_str(int num, int base, int uppercase) {
    if (base < 2 || base > 36) {
        base = 10;
    }
    
    const char* digits = uppercase ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";

    char* result = (char*)malloc(100 * sizeof(char)); 
    if (result == NULL) {
        printf("Ошибка выделения памяти.\n");
        return NULL;
    }

    int is_negative = num < 0;
    if (is_negative) { 
        num = -num; 
    }
    int i = 0;

    while (num != 0) {
        result[i++] = digits[num % base];
        num /= base;
    }

    if (is_negative) {
        result[i++] = '-';
    }

    for (int j = 0; j < i / 2; j++) {
        char temp = result[j];
        result[j] = result[i - 1 - j];
        result[i - 1 - j] = temp;
    }

    result[i] = '\0';

    return result;
}

int str_int(const char* str, int base, int uppercase) {
    if (base < 2 || base > 36) {
        base = 10;
    } 

    int result = 0;
    while (*str) {
        char c = uppercase ? toupper(*str) : *str;
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0'; 
        } else {
            digit = c - 'A' + 10;
        }
        result = result * base + digit;
        str++;
    }

    return result;
}

void memory_dump(void* ptr, int size, char* buffer) {
    char* bytes = (char*)ptr;
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            buffer += sprintf(buffer, "%d", (bytes[i] >> j) & 1);
        }
        buffer += sprintf(buffer, " ");
    }
}

int overfprintf(FILE* stream, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int written = 0;
    while (*format) {
        if (*format != '%') {
            fputc(*format, stream);
            written++;
        } else {
            format++;

            switch (*format) {
                case 'R': {
                    format++;
                    if (*format == 'o') {
                        int num = va_arg(args, int);
                        char* roman = int_roman(num);
                        if (roman) {
                            written += fprintf(stream, "%s", roman);
                            free(roman);
                        }
                    }
                    break;
                }
                case 'Z': {
                    format++;
                    if (*format == 'r') {
                        unsigned int num = va_arg(args, unsigned int);
                        char* zeckendorf = int_zeckendorf(num);
                        if (zeckendorf) {
                            written += fprintf(stream, "%s", zeckendorf);
                            free(zeckendorf);
                        }
                    }
                    break;
                }
                case 'C': {
                    format++;
                    if (*format == 'V' || *format == 'v') {
                        int uppercase = (*format == 'V'); 
                        int num = va_arg(args, int);
                        int base = va_arg(args, int);
                        char* base_str = int_str(num, base, uppercase);
                        if (base_str) { 
                            written += fprintf(stream, "%s", base_str);
                            free(base_str);
                        }
                    }
                    break;
                }
                case 't': {
                    format++;
                    if (*format == 'o') {
                        int uppercase = 1;
                        const char* str = va_arg(args, const char*);
                        int base = va_arg(args, int);
                        int num = str_int(str, base, uppercase);
                        written += fprintf(stream, "%d", num);
                        break;
                    }
                }
                case 'T': {
                    format++;
                    if (*format == 'O') {
                        int uppercase = 0;
                        const char* str = va_arg(args, const char*);
                        int base = va_arg(args, int);
                        int num = str_int(str, base, uppercase);
                        written += fprintf(stream, "%d", num);
                        break;
                    }
                }
                case 'm': {
                    format++;
                    if (*format == 'i') {
                        int num = va_arg(args, int);
                        char temp[33];
                        temp[0] = '\0';
                        memory_dump(&num, sizeof(int), temp);
                        written += fprintf(stream, "%s", temp);
                    } else if (*format == 'u') {
                        unsigned int num = va_arg(args, unsigned int);
                        char temp[33];
                        temp[0] = '\0';
                        memory_dump(&num, sizeof(unsigned int), temp);
                        written += fprintf(stream, "%s", temp);
                    } else if (*format == 'd') {
                        double num = va_arg(args, double);
                        char temp[65];
                        temp[0] = '\0';
                        memory_dump(&num, sizeof(double), temp);
                        written += fprintf(stream, "%s", temp);
                    } else if (*format == 'f') {
                        float num = (float)va_arg(args, double);
                        char temp[33];
                        temp[0] = '\0';
                        memory_dump(&num, sizeof(float), temp);
                        written += fprintf(stream, "%s", temp);
                    }
                    break;
                }
                default:
                    written += vfprintf(stream, format - 1, args);
                    while (*format != '\n'){
                        format++;
                    }
                    break;
            }
        }
        format++;
    }

    va_end(args);
    return written;
}

int oversprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int written = 0;
    while (*format) {
        if (*format != '%') {
            buffer[written++] = *format;
        } else {
            format++;
            switch (*format) {
                case 'R': {
                    format++;
                    if (*format == 'o') {
                        int num = va_arg(args, int);
                        char* roman = int_roman(num);
                        if (roman) {
                            written += sprintf(buffer + written, "%s", roman);
                            free(roman);
                        }
                    }
                    break;
                }
                case 'Z': {
                    format++;
                    if (*format == 'r') {
                        unsigned int num = va_arg(args, unsigned int);
                        char* zeckendorf = int_zeckendorf(num);
                        if (zeckendorf) {
                            written += sprintf(buffer + written, "%s", zeckendorf);
                            free(zeckendorf); 
                        }
                    }
                    break;
                }
                case 'C': {
                    format++;
                    if (*format == 'v' || *format == 'V') {
                        int uppercase = (*format == 'V');
                        int num = va_arg(args, int);
                        int base = va_arg(args, int);
                        char* base_str = int_str(num, base, uppercase);
                        written += sprintf(buffer + written, "%s", base_str);
                        free(base_str);
                        break;
                    }
                }
                case 't': {
                    format++;
                    int uppercase = 1;
                    const char* str = va_arg(args, const char*);
                    int base = va_arg(args, int);
                    int num = str_int(str, base, uppercase);
                    written += sprintf(buffer + written, "%d", num);
                    break;
                }
                case 'T': {
                    format++;
                    int uppercase = 0;
                    const char* str = va_arg(args, const char*);
                    int base = va_arg(args, int);
                    int num = str_int(str, base, uppercase);
                    written += sprintf(buffer + written, "%d", num);
                    break;
                }
                case 'm': {
                    format++;
                    if (*format == 'i') {
                        int num = va_arg(args, int);
                        char temp[33];
                        temp[0] = '\0';
                        memory_dump(&num, sizeof(int), temp);
                        written += sprintf(buffer + written, "%s", temp);
                    } else if (*format == 'u') {
                        unsigned int num = va_arg(args, unsigned int);
                        char temp[33];
                        temp[0] = '\0';
                        memory_dump(&num, sizeof(unsigned int), temp);
                        written += sprintf(buffer + written, "%s", temp);
                    } else if (*format == 'd') {
                        double num = va_arg(args, double);
                        char temp[65];
                        temp[0] = '\0';
                        memory_dump(&num, sizeof(double), temp);
                        written += sprintf(buffer + written, "%s", temp);
                    } else if (*format == 'f') {
                        double num = va_arg(args, double);
                        char temp[33];
                        temp[0] = '\0';
                        memory_dump(&num, sizeof(float), temp);
                        written += sprintf(buffer + written, "%s", temp);
                    }
                    break;
                }
                default:
                    written += vsprintf(buffer + written, format - 1, args);
                    while (*format != '\n'){
                        format++;
                    }
                    break;
            }
        }
        format++;
    }

    buffer[written] = '\0';
    va_end(args);
    return written;
}

int main() {
    int a = 5;

    overfprintf(stdout, "Целое число, записанное римскими цифрами: %Ro\n", 3888);
    overfprintf(stdout, "Цекендорфово представление числа: %Zr\n", 10);
    overfprintf(stdout, "Перевод из целого числа в строковое: %Cv\n", 255, 16);
    overfprintf(stdout, "Перевод из целого числа в строковое: %CV\n", 255, 16);
    overfprintf(stdout, "Перевод из строкового числа в целое: %to\n", "ff", 16);
    overfprintf(stdout, "Перевод из строкового числа в целое: %TO\n", "FF", 16);
    overfprintf(stdout, "Печать целого числа: %p\n", &a);
    overfprintf(stdout, "Печать строкового числа: %X\n", 255);
    overfprintf(stdout, "Дамп памяти: %mi\n", -25);
    overfprintf(stdout, "Дамп памяти: %mu\n", 25);
    overfprintf(stdout, "Дамп памяти: %md\n", 3.14);
    overfprintf(stdout, "Дамп памяти: %mf\n", 3.14);

    char buffer[256];
    oversprintf(buffer, "Целое число, записанное римскими цифрами: %Ro\n", 546);
    printf("%s", buffer);

    return 0;
}