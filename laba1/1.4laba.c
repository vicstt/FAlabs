#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum kOpts {
    OPT_INVALID = -1,
    OPT_D,
    OPT_I,
    OPT_S,
    OPT_A
} kOpts;

int GetOpts(int argc, char** argv, kOpts *option, char** inputFile, char** outputFile) {
    if (argc < 3) {
        *option = OPT_INVALID;
        return 1;
    }

    *inputFile = argv[2];

    if (argv[1][0] == '-' || argv[1][0] == '/') {
        if (argv[1][1] == 'd' || argv[1][2] == 'd') {
            *option = OPT_D;
            if (argc == 4 && argv[1][1] == 'n') {
                *outputFile = argv[3];
            } else {
                *outputFile = malloc(strlen(*inputFile) + 5);
                strcpy(*outputFile, "out_");
                strcat(*outputFile, *inputFile);
            }
            return 0;
        } else if (argv[1][1] == 'i' || argv[1][2] == 'i') {
            *option = OPT_I;
            if (argc == 4 && argv[1][1] == 'n') {
                *outputFile = argv[3];
            } else {
                *outputFile = malloc(strlen(*inputFile) + 5);
                strcpy(*outputFile, "out_");
                strcat(*outputFile, *inputFile);
            }
            return 0;
        } else if (argv[1][1] == 's' || argv[1][2] == 's') {
            *option = OPT_S;
            if (argc == 4 && argv[1][1] == 'n') {
                *outputFile = argv[3];
            } else {
                *outputFile = malloc(strlen(*inputFile) + 5);
                strcpy(*outputFile, "out_");
                strcat(*outputFile, *inputFile);
            }
            return 0;
        } else if (argv[1][1] == 'a' || argv[1][2] == 'a') {
            *option = OPT_A;
            if (argc == 4 && argv[1][1] == 'n') {
                *outputFile = argv[3];
            } else {
                *outputFile = malloc(strlen(*inputFile) + 5);
                strcpy(*outputFile, "out_");
                strcat(*outputFile, *inputFile);
            }
            return 0;
        }
    }

    *option = OPT_INVALID;
    return 1;
}

void HandlerOptD(char* inputFile, char* outputFile) {
    FILE* inFile = fopen(inputFile, "r");
    FILE* outFile = fopen(outputFile, "w");
    if (inFile == NULL || outFile == NULL) {
        printf("Ошибка открытия файлов.\n");
        return;
    }
    char symbol;
    while ((symbol = fgetc(inFile)) != EOF) {
        if (!isdigit(symbol)) {
            fputc(symbol, outFile);
        }
    }
    fclose(inFile);
    fclose(outFile);
}

void HandlerOptI(char* inputFile, char* outputFile) {
    FILE* inFile = fopen(inputFile, "r");
    FILE* outFile = fopen(outputFile, "w");
    if (inFile == NULL || outFile == NULL) {
        printf("Ошибка открытия файлов.\n");
        return;
    }
    char line[1000];
    while (fgets(line, 1000, inFile) != NULL) {
        int count = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (isalpha(line[i])) {
                count++;
            }
        }
        fprintf(outFile, "%d\n", count);
    }
    fclose(inFile);
    fclose(outFile);
}

void HandlerOptS(char* inputFile, char* outputFile) {
    FILE* inFile = fopen(inputFile, "r");
    FILE* outFile = fopen(outputFile, "w");
    if (inFile == NULL || outFile == NULL) {
        printf("Ошибка открытия файлов.\n");
        return;
    }
    char line[1000];
    while (fgets(line, 1000, inFile) != NULL) {
        int count = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isalpha(line[i]) && !isdigit(line[i]) && line[i] != ' ') {
                count++;
            }
        }
        fprintf(outFile, "%d\n", count);
    }
    fclose(inFile);
    fclose(outFile);
}

void HandlerOptA(char* inputFile, char* outputFile) {
    FILE* inFile = fopen(inputFile, "r");
    FILE* outFile = fopen(outputFile, "w");
    if (inFile == NULL || outFile == NULL) {
        printf("Ошибка открытия файлов.\n");
        return;
    }
    char symbol;
    while ((symbol = fgetc(inFile)) != EOF) {
        if (!isdigit(symbol)) {
            fprintf(outFile, "%02X ", (unsigned char)symbol);
        } else {
            fputc(symbol, outFile);
        }
    }
    fclose(inFile);
    fclose(outFile);
}

int main(int argc, char** argv) {
    kOpts opt = OPT_INVALID;
    char* inputFile = NULL;
    char* outputFile = NULL;

    void (*handlers[4])(char*, char*) = {
        HandlerOptD,
        HandlerOptI,
        HandlerOptS,
        HandlerOptA
    };

    if (GetOpts(argc, argv, &opt, &inputFile, &outputFile)) {
        printf("Неверный флаг или количество аргументов.\n");
        return 1;
    }

    handlers[opt](inputFile, outputFile);
    if (outputFile != argv[3]) {
        free(outputFile); 
    }
    return 0;
}
