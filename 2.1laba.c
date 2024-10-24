#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

typedef enum kOpts {
    OPT_L, 
    OPT_R,
    OPT_U, 
    OPT_N, 
    OPT_C, 
    OPT_INVALID
} kOpts;

int str_len(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int GetOpts(int argc, char** argv, kOpts *option, int *number) {
    if (argc < 3) {
        return 1;
    }

    if (argv[1][0] == '-' && argv[1][1] == 'l') {
        *option = OPT_L;
    } else if (argv[1][0] == '-' && argv[1][1] == 'r') {
        *option = OPT_R;
    } else if (argv[1][0] == '-' && argv[1][1] == 'u') {
        *option = OPT_U;
    } else if (argv[1][0] == '-' && argv[1][1] == 'n') {
        *option = OPT_N;
    } else if (argv[1][0] == '-' && argv[1][1] == 'c') {
        *option = OPT_C;
        if (argc < 4) {
            return 1; 
        }
    } else {
        *option = OPT_INVALID;
        return 1; 
    }

    return 0; 
}

char *HandlerOptL(char *str) { // подсчет длины строки
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    char *result = malloc(sizeof(char) * (length + 1)); 
    if (result == NULL) {
        return NULL;
    }
    sprintf(result, "%d", length); 
    return result;
}

char *HandlerOptR(char *str) { // reverse строки
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    char *result = malloc(sizeof(char) * (length + 1));
    if (result == NULL) {
        return NULL;
    }
    int i = 0;
    int j = length - 1;
    while (i < j) {
        result[i] = str[j];
        result[j] = str[i];
        i++;
        j--;
    }
    if (i == j) {
        result[i] = str[i];
    }
    result[length] = '\0';
    return result;
}

char *HandlerOptU(char *str) { // преобразование символов на нечётных позициях в верхний регистр
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    char *result = malloc(sizeof(char) * (length + 1));
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        if (i % 2 == 1) {
            result[i] = toupper(str[i]);
        } else {
            result[i] = str[i];
        }
    }
    result[length] = '\0';
    return result;
}

char *HandlerOptN(char *str) { // вывести строку в порядке: цифры, буквы, все остальное
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    char *result = malloc(sizeof(char) * (length + 1));
    if (result == NULL) {
        return NULL;
    }
    int digits_count = 0;
    int letters_count = 0;
    int others_count = 0;
    for (int i = 0; i < length; i++) {
        if (isdigit(str[i])) {
            digits_count++;
        } else if (isalpha(str[i])) {
            letters_count++;
        } else {
            others_count++;
        }
    }
    int digits_index = 0;
    int letters_index = digits_count;
    int others_index = digits_count + letters_count;
    for (int i = 0; i < length; i++) {
        if (isdigit(str[i])) {
            result[digits_index++] = str[i];
        } else if (isalpha(str[i])) {
            result[letters_index++] = str[i];
        } else {
            result[others_index++] = str[i];
        }
    }
    result[length] = '\0';
    return result;
}

char *HandlerOptC(int argc, char** argv, int seed) { // конкатенация строк
    if (argc < 4) { 
        return NULL;
    }

    int length = 0;
    for (int i = 3; i < argc; i++) { 
        length += str_len(argv[i]);
    }

    char *result = (char*)malloc(sizeof(char) * (length + 1));
    if (result == NULL) {
        return NULL;
    }

    srand(seed);

    int *used = (int*)malloc(sizeof(int) * (argc - 3));
    if (used == NULL) {
        free(result);
        return NULL;
    }
    for (int i = 0; i < argc - 3; i++) {
        used[i] = 0;
    }

    int resindex = 0; 
    for (int i = 0; i < argc - 3; i++) { 
        int rindex = rand() % (argc - 3); 
        while (used[rindex] == 1) {
            rindex = rand() % (argc - 3);
        }
        used[rindex] = 1;
        for (int j = 0; argv[3 + rindex][j] != '\0'; j++) { 
            result[resindex++] = argv[3 + rindex][j];
        }
    }

    result[length] = '\0';
    free(used);

    return result;
}

int main (int argc, char** argv) {
    kOpts opt = OPT_INVALID;
    int procceed_number = 0;
    char *result = NULL;

    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("Неверный флаг или количество аргументов.\n");
        return 1;
    }

    switch (opt) {
        case OPT_L:
            result = HandlerOptL(argv[2]);
            break;
        case OPT_R:
            result = HandlerOptR(argv[2]);
            break;
        case OPT_U:
            result = HandlerOptU(argv[2]);
            break;
        case OPT_N:
            result = HandlerOptN(argv[2]);
            break;
        case OPT_C:
            bool flag = true;
            unsigned int seed = 0;
            for (int i = 0; argv[2][i] != '\0'; i++) {
                if (!isdigit(argv[2][i])) {
                    flag = false;
                    break;
                }
            }
            if (flag == true){
                seed = atoi(argv[2]);
                result = HandlerOptC(argc, argv, seed);
                break;
            }
            else{
                printf("Ошибка: seed должен быть целым числом.\n");
                break;
            }
        default:
            printf("Неверный флаг.\n");
            return 1;
    }

    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    return 0;
}
