#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int is_kaprekar(int n) {
    if (n < 0) return 0;
    long long squared = (long long)n * n;
    char str[20], left[20], right[20];
    sprintf(str, "%lld", squared);
    int len = strlen(str);
    int split = len / 2;
    strcpy(right, str + split);
    right[len - split] = '\0';
    if (split > 0) {
        strncpy(left, str, split);
        left[split] = '\0';
    } else {
        strcpy(left, "0");
    }

    int left_num = atoi(left);
    int right_num = atoi(right);
    return (left_num + right_num == n);
}

void find_kaprekar(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    printf("Числа Капрекара в системе счисления с основанием %d:\n", base);

    for (int i = 0; i < count; i++) {
        char *number_str = va_arg(args, char *);
        int num = strtol(number_str, NULL, base);

        if (is_kaprekar(num)) {
            printf("%s\n", number_str);
        }
    }

    va_end(args);
}

int main() {
    find_kaprekar(10, 5, "1", "9", "45", "55", "297"); 

    return 0;
}
