#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum kOpts {
    OPT_R,
    OPT_A,
    OPT_INVALID = -1
} kOpts;

char *to_base4(int num) { // преобразования числа в строку в системе счисления с основанием 4
    char digits[] = "0123";
    int len = 0;
    int temp = num;

    while (temp) {
        temp /= 4;
        len++;
    }

    char *result = malloc(len + 1);
    if (result != NULL) {
        temp = num;
        for (int i = len - 1; i >= 0; i--) {
            result[i] = digits[temp % 4];
            temp /= 4;
        }
        result[len] = '\0';
    }
    return result;
}

char *to_base8(int num) { // преобразование числа в строку в системе счисления с основанием 8
    char digits[] = "01234567";
    int len = 0;
    int temp = num;

    while (temp) {
        temp /= 8;
        len++;
    }

    char *result = malloc(len + 1);
    if (result != NULL) {
        temp = num;
        for (int i = len - 1; i >= 0; i--) {
            result[i] = digits[temp % 8];
            temp /= 8;
        }
        result[len] = '\0';
    }
    return result;
}

int GetOpts(int argc, char** argv, kOpts *option, int *number) {
    if (argc < 4 || argc > 6) {
        *option = OPT_INVALID;
        return 1;
    }
    char *flag = argv[1];
    if (argv[1][0] == '-' || argv[1][0] == '/') {
        if (argv[1][1] == 'r') {
            *option = OPT_R;
            *number = argc - 1;
            return 0;
        } else if (argv[1][1] == 'a') {
            *option = OPT_A;
            *number = argc - 1;
            return 0;
        }
    }
    *option = OPT_INVALID;
    return 1;
}

void HandlerOptR(int number, char **argv) {
    if (number != 4) {
        printf("Неверное количество аргументов для опции -r.\n");
        return;
    }
    char *f1 = argv[2];
    char *f2 = argv[3];
    char *fo = argv[4];
    char word1[100];
    char word2[100];

    FILE *file1 = fopen(f1, "r");
    if (file1 == NULL) {
        printf("Ошибка открытия файла %s\n", argv[1]);
        return;
    }

    FILE *file2 = fopen(f2, "r");
    if (file2 == NULL) {
        printf("Ошибка открытия файла %s\n", argv[2]);
        fclose(file1);
        return;
    }

    FILE *output = fopen(fo, "w");
    if (output == NULL) {
        printf("Ошибка открытия файла %s\n", argv[3]);
        fclose(file1);
        fclose(file2);
        return;
    }

    while (fscanf(file1, "%s", word1) != EOF && fscanf(file2, "%s", word2) != EOF) {
        fprintf(output, "%s ", word1);
        fprintf(output, "%s ", word2);
    }

    if (!feof(file1)) {
        fprintf(output, "%s ", word1);
        while (fscanf(file1, "%s", word1) != EOF) {
            fprintf(output, "%s ", word1);
        }   
    }

    if (!feof(file2)) {
        word2[0] = '\0'; 
        fprintf(output, "%s ", word2);
        while (fscanf(file2, "%s", word2) != EOF) {
            fprintf(output, "%s ", word2);
        }
    }

    fclose(file1);
    fclose(file2);
    fclose(output);
}

void HandlerOptA(int number, char **argv) {
    if (number != 3) {
        printf("Неверное количество аргументов для опции -q.\n");
        return;
    }
    char *file1 = argv[2];
    char *outfile = argv[3];
    char *token;

    FILE *fp1 = fopen(file1, "r");
    if (fp1 == NULL) {
        printf("Ошибка открытия файла %s\n", file1);
        return;
    }

    FILE *fpout = fopen(outfile, "w");
    if (fpout == NULL) {
        printf("Ошибка открытия файла %s\n", outfile);
        return;
    }
    char buffer[1000];
    int i = 1;
    while (fgets(buffer, sizeof(buffer), fp1) != NULL) {
        token = strtok(buffer, " \t\n");
        while (token != NULL) {
            if (i % 10 == 0) {
                for (int j = 0; token[j] != '\0'; j++) { // перевод в строчные буквы
                    token[j] = tolower(token[j]);
                }
                for (int j = 0; token[j] != '\0'; j++) { // перевод в ASCII-код и в сc с основанием 4
                    char *base4 = to_base4((int)token[j]);
                    fprintf(fpout, "%s ", base4);
                    free(base4);
                }
            } else if (i % 2 == 0 && i % 10 != 0) {
                for (int j = 0; token[j] != '\0'; j++) { // перевод в строчные буквы
                    token[j] = tolower(token[j]);
                }
                fprintf(fpout, "%s ", token);
            } else if (i % 5 == 0 && i % 10 != 0) {
                for (int j = 0; token[j] != '\0'; j++) { // перевод в ASCII-код и в сc с основанием 8
                    char *base8 = to_base8((int)token[j]);
                    fprintf(fpout, "%s ", base8);
                    free(base8);
                }
            } else {
                fprintf(fpout, "%s ", token);
            }
            token = strtok(NULL, " \t\n");
            i++;
        }
    }
    fclose(fp1);
    fclose(fpout);
}

int main(int argc, char** argv) {
    kOpts opt = OPT_INVALID;
    int x = 0;
    void (*handlers[2])(int, char**) = { // массив указателей на функции
        HandlerOptR,
        HandlerOptA
    };
    if (GetOpts(argc, argv, &opt, &x)) {
        printf("Неверный флаг или количество аргументов.\n");
        return 1;
    }
    handlers[opt](x, argv);
    return 0;
}
