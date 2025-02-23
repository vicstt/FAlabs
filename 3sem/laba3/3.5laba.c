#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAM_COUNT 5

typedef struct {
    unsigned int id;
    char name[50];
    char surname[50];
    char group[20];
    unsigned char* grades;
} Student;

void load_students(const char* filename, Student** students, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка при открытии файла");
        return;
    }

    *count = 0;
    *students = NULL;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        *students = realloc(*students, (*count + 1) * sizeof(Student));
        Student* s = &(*students)[*count];

        sscanf(line, "%u %s %s %s", &s->id, s->name, s->surname, s->group);
        s->grades = (unsigned char*)malloc(EXAM_COUNT * sizeof(unsigned char));

        char* token = strtok(line, " ");
        for (int j = 0; j < 3; j++) {
            token = strtok(NULL, " ");
        }
        for (int j = 0; j < EXAM_COUNT; j++) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                s->grades[j] = (unsigned char)atoi(token);
            }
        }

        (*count)++;
    }
    fclose(file);
}

double student_average(Student* student) {
    if (student == NULL || student->grades == NULL) {
        return 0.0; 
    }

    double sum = 0;
    for (int i = 0; i < EXAM_COUNT; i++) {
        sum += student->grades[i];
    }
    return sum / EXAM_COUNT;
}

void print_student(FILE* log_file, Student* student) {
    if (!student) return;

    fprintf(log_file, "ID: %u, Имя: %s, Фамилия: %s, Группа: %s, Средний балл: %.2f\n",
            student->id, student->name, student->surname, student->group,
            student_average(student));
}

int compare_id(const void* a, const void* b) {
    return ((Student*)a)->id - ((Student*)b)->id;
}

int compare_surname(const void* a, const void* b) {
    return strcmp(((Student*)a)->surname, ((Student*)b)->surname);
}

int compare_name(const void* a, const void* b) {
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}

int compare_group(const void* a, const void* b) {
    return strcmp(((Student*)a)->group, ((Student*)b)->group);
}

void print_average(FILE* log_file, Student* students, int count, double overall_avg) {
    for (int i = 0; i < count; i++) {
        double avg = student_average(&students[i]);
        if (avg > overall_avg) {
            fprintf(log_file, "ID: %u, Имя: %s, Фамилия: %s, Средний балл: %.2f\n", students[i].id, students[i].name, students[i].surname, avg);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Использование: %s <файл данных> <файл трассировки> <файл команд>\n", argv[0]);
        return 1;
    }

    const char* data_file = argv[1];
    const char* log_file_path = argv[2];
    const char* commands_file = argv[3];

    Student* students = NULL;
    int count = 0;
    load_students(data_file, &students, &count);

    FILE* log_file = fopen(log_file_path, "w");
    if (!log_file) {
        perror("Ошибка при открытии файла трассировки");
        free(students);
        return 1;
    }

    FILE* cmd_file = fopen(commands_file, "r");
    if (!cmd_file) {
        perror("Ошибка при открытии файла команд");
        fclose(log_file);
        free(students);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), cmd_file)) {
        int choice;
        unsigned int id;
        char name[50];
        char surname[50];
        char group[50];
        Student* student;

        char* token = strtok(line, " ");
        if (token != NULL) {
            choice = atoi(token);
            if (choice == 0) break;

            switch (choice) {
                case 1:
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        id = (unsigned int)atoi(token);
                        Student* student = NULL;
                        for (int i = 0; i < count; i++) {
                            if (students[i].id == id) {
                                student = &students[i];
                                break;
                            }
                        }
                        if (student) {
                            fprintf(log_file, "Студенты с ID %u:\n", id);
                            print_student(log_file, student);
                        } else {
                            fprintf(log_file, "Не найдено.\n");
                        }
                    }
                    fprintf(log_file, "\n");
                    break;
                case 2:
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        strncpy(name, token, sizeof(name) - 1);
                        name[sizeof(name) - 1] = '\0';
                        size_t len = strlen(name);
                        if (len > 0 && name[len - 1] == '\n') {
                            name[len - 1] = '\0';
                        }
                        fprintf(log_file, "Студенты с именем %s:\n", name);
                        int found = 0;
                        for (int i = 0; i < count; i++) {
                            if (strcmp(students[i].name, name) == 0) {
                                print_student(log_file, &students[i]);
                                found = 1;
                            }
                        }
                        if (!found) {
                            fprintf(log_file, "Не найдено.\n");
                        }
                    }
                    fprintf(log_file, "\n");
                    break;
                case 3:
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        strncpy(surname, token, sizeof(surname) - 1);
                        surname[sizeof(surname) - 1] = '\0';
                        size_t len = strlen(surname);
                        if (len > 0 && surname[len - 1] == '\n') {
                            surname[len - 1] = '\0';
                        }
                        fprintf(log_file, "Студенты с фамилией %s:\n", surname);
                        int found = 0;
                        for (int i = 0; i < count; i++) {
                            if (strcmp(students[i].surname, surname) == 0) {
                                print_student(log_file, &students[i]);
                                found = 1;
                            }
                        }
                        if (!found) {
                            fprintf(log_file, "Не найдено.\n");
                        }
                    }
                    fprintf(log_file, "\n");
                    break;
                case 4:
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        strncpy(group, token, sizeof(group) - 1);
                        group[sizeof(group) - 1] = '\0';
                        size_t len = strlen(group);
                        if (len > 0 && group[len - 1] == '\n') {
                            group[len - 1] = '\0';
                        }
                        fprintf(log_file, "Студенты с группой %s:\n", group);
                        int found = 0;
                        for (int i = 0; i < count; i++) {
                            if (strcmp(students[i].group, group) == 0) {
                                print_student(log_file, &students[i]);
                                found = 1;
                            }
                        }
                        if (!found) {
                            fprintf(log_file, "Не найдено.\n");
                        }
                    }
                    fprintf(log_file, "\n");
                    break;
                case 5:
                    qsort(students, count, sizeof(Student), compare_id);
                    fprintf(log_file, "Студенты отсортированы по ID:\n");
                    student = NULL;
                    for (int i = 0; i < count; i++) {
                        student = &students[i];
                        print_student(log_file, student);
                    }
                    fprintf(log_file, "\n");
                    break;
                case 6:
                    qsort(students, count, sizeof(Student), compare_surname);
                    fprintf(log_file, "Студенты отсортированы по фамилии:\n");
                    student = NULL;
                    for (int i = 0; i < count; i++) {
                        student = &students[i];
                        print_student(log_file, student);
                    }
                    fprintf(log_file, "\n");
                    break;
                case 7:
                    qsort(students, count, sizeof(Student), compare_name);
                    fprintf(log_file, "Студенты отсортированы по имени:\n");
                    student = NULL;
                    for (int i = 0; i < count; i++) {
                        student = &students[i];
                        print_student(log_file, student);
                    }
                    fprintf(log_file, "\n");
                    break;
                case 8:
                    qsort(students, count, sizeof(Student), compare_group);
                    fprintf(log_file, "Студенты отсортированы по группе:\n");
                    student = NULL;
                    for (int i = 0; i < count; i++) {
                        student = &students[i];
                        print_student(log_file, student);
                    }
                    fprintf(log_file, "\n");
                    break;
                case 9:
                    double overall_avg = 0;
                    for (int i = 0; i < count; i++) {
                        overall_avg += student_average(&students[i]);
                    }
                    overall_avg /= count;
                    fprintf(log_file, "Средний балл по всем студентам: %.2f\n", overall_avg);
                    print_average(log_file, students, count, overall_avg);
                    fprintf(log_file, "\n");
                    break;
                default:
                    fprintf(log_file, "Неверный выбор!\n");
                    fprintf(log_file, "\n");
            }
        }
    }

    fclose(cmd_file);
    fclose(log_file);
    for (int i = 0; i < count; i++) {
        free(students[i].grades);
    }
    free(students);

    return 0;
}
