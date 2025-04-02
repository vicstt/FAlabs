#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    unsigned int id;
    char* name;
    char* surname;
    double wages;
} Employee;

char* strdup(char *str) {
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

int compare_employees_incr(const void *a, const void *b){
    Employee* empl1 = (Employee*)a;
    Employee* empl2 = (Employee*)b;
    if (empl1->wages != empl2->wages){
        return (empl1->wages > empl2->wages) ? 1 : -1;
    }
    else if (strcmp(empl1->surname, empl2->surname) != 0){
        return strcmp(empl1->surname, empl2->surname);
    }
    else if (strcmp(empl1->name, empl2->name) != 0){
        return strcmp(empl1->name, empl2->name);
    }
    else {
        return (empl1->id > empl2->id) ? 1 : -1;
    }
}

int compare_employees_decr(const void *a, const void *b){
    Employee* empl1 = (Employee*)a;
    Employee* empl2 = (Employee*)b;
    if (empl1->wages != empl2->wages){
        return (empl1->wages > empl2->wages) ? -1 : 1;
    }
    else if (strcmp(empl1->surname, empl2->surname) != 0){
        return -strcmp(empl1->surname, empl2->surname);
    }
    else if (strcmp(empl1->name, empl2->name) != 0){
        return -strcmp(empl1->name, empl2->name);
    }
    else {
        return (empl1->id > empl2->id) ? -1 : 1;
    }
}

int main(int argc, char **argv) {
    if (argc != 4){
        printf("Неверное количество аргуметов.\n");
        return 1;
    }

    char *inputfile = argv[1];
    char *flag = argv[2];
    char *outputfile = argv[3];

    FILE *input = fopen(inputfile, "r");
    if (input == NULL){
        printf("Не удалось открыть файл %s.\n", inputfile);
    }

    FILE *output = fopen(outputfile, "w");
    if (output == NULL){
        printf("Не удалось открыть файл %s.\n", outputfile);
    }

    Employee *employees = NULL;
    int empl_cnt = 0;
    char buffer[1000];

    while(fgets(buffer, sizeof(buffer), input)){
        empl_cnt++;
        employees = (Employee*)(realloc(employees, sizeof(Employee)*empl_cnt));
        char* token = strtok(buffer, " ");
        employees[empl_cnt - 1].id = atoi(token);
        token = strtok(NULL, " ");
        employees[empl_cnt - 1].name = strdup(token);
        token = strtok(NULL, " ");
        employees[empl_cnt - 1].surname = strdup(token);
        token = strtok(NULL, " ");
        employees[empl_cnt - 1].wages = atof(token);
    }

    if (flag[0] == '-' || flag[0] == '/'){
        if (flag[1] == 'a'){
            qsort(employees, empl_cnt, sizeof(Employee), compare_employees_incr);
        }
        else if(flag[1] == 'd'){
            qsort(employees, empl_cnt, sizeof(Employee), compare_employees_decr);
        }
        else{
            printf("Неверный флаг.\n");
            return 1;
        }
    } else{
        printf("Неверный флаг.\n");
        return 1;
    }

    for (int i = 0; i < empl_cnt; i++) { 
        fprintf(output, "%d %s %s %.2f\n", employees[i].id, employees[i].name, employees[i].surname, employees[i].wages);
    }

    for (int i = 0; i < empl_cnt; i++) {  
        free(employees[i].name);
        free(employees[i].surname);
    }
    free(employees);
    fclose(input);
    fclose(output);
    return 0;
}