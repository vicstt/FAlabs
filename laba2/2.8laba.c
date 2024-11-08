#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

char* str_copy(const char *str) {
    if (str == NULL) {
        return NULL;
    }
 char *dup = (char*) malloc(strlen(str) + 1);
    if (dup == NULL) {
        return NULL; 
    }
    strcpy(dup, str);
    
    return dup; 
}

int char_int(char num) { 
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

char int_char(int num) {
    if (num < 10) {
        return '0' + num;
    } else {
        return 'A' + (num - 10);
    }
}

char* add_in_base(const char* num1, const char* num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int max_len = len1 > len2 ? len1 : len2;
    char* result = (char*)malloc(max_len + 2); 
    if (!result) return NULL; 

    int carry = 0;
    int n1 = len1 - 1;
    int n2 = len2 - 1;
    int res = max_len;

    result[res + 1] = '\0'; 

    while (n1 >= 0 || n2 >= 0 || carry > 0) {
        int digit1 = (n1 >= 0) ? char_int(num1[n1--]) : 0;
        int digit2 = (n2 >= 0) ? char_int(num2[n2--]) : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        result[res--] = int_char(sum % base);
    }

    int zeros = 0;
    if (result[0] == '0') {
        zeros++;
    }

    while (result[res + 1 + zeros] == '0' && zeros < max_len) {
        zeros++;
    }

    if (zeros == max_len) {
        result[0] = '0';
        result[1] = '\0';
    } else {
        memmove(result, result + res + 1 + zeros, max_len - res - zeros);
        result[max_len - res - zeros] = '\0';
    }

    return result;
}

char* sum(int base, int count, ...) {
    va_list args;
    va_start(args, count);
    if (base < 2 || base > 36) {
        va_end(args);
        return NULL; 
    }
    char *summa = NULL;
    char *current_num;
    
    for (int i = 0; i < count; i++) {
        current_num = va_arg(args, char*);
        if (summa == NULL) {
            summa = str_copy(current_num);
        } else {
            char *new_summa = add_in_base(summa, current_num, base);
            free(summa);
            summa = new_summa;
        }
    }
    
    va_end(args);
    return summa;
}

int main() {
    char* result = sum(10, 3, "999", "1", "00010");
    printf("Сумма: %s\n", result);
    free(result);

    return 0;
}