#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Liver {
    char surname[50];
    char name[50];
    char patronymic[50];
    int day, month, year;
    char gender;
    double income;
} Liver;

typedef struct Node {
    Liver data;
    struct Node* next;
} Node;

Node* create(Liver data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void insert(Node** head, Liver data) {
    Node* new_node = create(data);
    if (*head == NULL || (*head)->data.year < data.year ||
        ((*head)->data.year == data.year && (*head)->data.month < data.month) ||
        ((*head)->data.year == data.year && (*head)->data.month == data.month && (*head)->data.day < data.day)) {
        new_node->next = *head;
        *head = new_node;
    } else {
        Node* current = *head;
        while (current->next != NULL &&
               (current->next->data.year > data.year ||
                (current->next->data.year == data.year && current->next->data.month > data.month) ||
                (current->next->data.year == data.year && current->next->data.month == data.month && current->next->data.day > data.day))) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void delete(Node** head, Liver data) {
    Node* temp = *head;
    Node* prev = NULL;

    if (temp != NULL &&
        strcmp(temp->data.surname, data.surname) == 0 &&
        strcmp(temp->data.name, data.name) == 0 &&
        strcmp(temp->data.patronymic, data.patronymic) == 0 &&
        temp->data.day == data.day &&
        temp->data.month == data.month &&
        temp->data.year == data.year &&
        temp->data.gender == data.gender &&
        temp->data.income == data.income) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL &&
           !(strcmp(temp->data.surname, data.surname) == 0 &&
             strcmp(temp->data.name, data.name) == 0 &&
             strcmp(temp->data.patronymic, data.patronymic) == 0 &&
             temp->data.day == data.day &&
             temp->data.month == data.month &&
             temp->data.year == data.year &&
             temp->data.gender == data.gender &&
             temp->data.income == data.income)) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return;
    }
    prev->next = temp->next;
    free(temp);
}

Node* find(Node* head, Liver data) {
    Node* current = head;
    while (current != NULL) {

        if (strcmp(data.surname, "") != 0) { 
            if (strcmp(current->data.surname, data.surname) != 0) {
                current = current->next;
                continue;
            }
        }

        if (strcmp(data.name, "") != 0) {
            if (strcmp(current->data.name, data.name) != 0) {
                current = current->next;
                continue;
            }
        }
    
        if (strcmp(data.patronymic, "") != 0) {
            if (strcmp(current->data.patronymic, data.patronymic) != 0) {
                current = current->next;
                continue;
            }
        }

        if (data.day != 0 || data.month != 0 || data.year != 0) { 
            if (data.day != 0 && current->data.day != data.day) {
                current = current->next;
                continue;
            }
            if (data.month != 0 && current->data.month != data.month) {
                current = current->next;
                continue;
            }
            if (data.year != 0 && current->data.year != data.year) {
                current = current->next;
                continue;
            }
        }

        if (data.gender != '\0') { 
            if (current->data.gender != data.gender) {
                current = current->next;
                continue;
            }
        }

        if (data.income != -1) { 
            if (current->data.income != data.income) {
                current = current->next;
                continue;
            }
        }

        return current;
    }
    return NULL;
}

void modify(Node* node, Liver new_data) {
    if (node == NULL) {
        return;
    }
    node->data = new_data;
}

void print_liver(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("Фамилия: %s, Имя: %s, Отчество: %s, Дата рождения: %d.%d.%d, Пол: %c, Доход за месяц: %.2f\n",
               current->data.surname, current->data.name, current->data.patronymic,
               current->data.day, current->data.month, current->data.year,
               current->data.gender, current->data.income);
        current = current->next;
    }
}

void save_to_file(Node* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %s %d %d %d %c %.2f\n",
                current->data.surname, current->data.name, current->data.patronymic,
                current->data.day, current->data.month, current->data.year,
                current->data.gender, current->data.income);
        current = current->next;
    }

    fclose(file);
}

void read_from_file(Node** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        Liver liver;
        sscanf(line, "%s %s %s %d %d %d %c %lf",
               liver.surname, liver.name, liver.patronymic,
               &liver.day, &liver.month, &liver.year,
               &liver.gender, &liver.income);
        insert(head, liver);
    }

    fclose(file);
}

typedef struct {
    Liver data;
    int type;
} Action;

typedef struct {
    Action* actions;
    int top;
    int capacity;
} Stack;

void init_stack(Stack* stack, int initial_capacity) {
    stack->actions = (Action*)malloc(initial_capacity * sizeof(Action));
    stack->top = 0;
    stack->capacity = initial_capacity;
}

void push(Stack* stack, Action action) {
    if (stack->top >= stack->capacity) {
        stack->capacity *= 2;
        stack->actions = (Action*)realloc(stack->actions, stack->capacity * sizeof(Action));
    }
    stack->actions[stack->top++] = action;
}

Action pop(Stack* stack) {
    if (stack->top > 0) {
        return stack->actions[--stack->top];
    }
    Action empty = {0};
    return empty;
}

void free_stack(Stack* stack) {
    free(stack->actions);
    stack->actions = NULL;
    stack->top = 0;
    stack->capacity = 0;
}

void undo(Node** head, Liver liver, Stack* stack, int count) {
    if (stack->top == 0) {
        printf("Стек пуст.\n");
        return;
    }

    if (count == 0) {
        printf("Недостаточно действий для отмены.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (stack->top == 0) {
            break;
        }
        Action action = pop(stack);
        if (action.type == 1) {
            delete(head, action.data);
        } else if (action.type == 2) {
            insert(head, action.data);
        } else if (action.type == 3) {
            Node* node = find(*head, liver);
            if (node != NULL) { 
                modify(node, action.data);
            }
        }
    }
}


void commands(Node** head, Stack* stack, const char* filename) {
    int modifications = 0;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    int buffer_size = 1000;
    char *line = (char*)malloc(buffer_size);
    if (line == NULL) {
        printf("Ошибка выделения памяти.\n");
        fclose(file);
        return;
    }

    while (fgets(line, buffer_size, file)) {
        line[strcspn(line, "\n")] = 0;

        while (strlen(line) == buffer_size - 1 && line[buffer_size - 2] != '\n') {
            buffer_size *= 2;
            line = (char*)realloc(line, buffer_size);
            if (line == NULL) {
                printf("Ошибка выделения памяти.\n");
                fclose(file);
                return;
            }
        }

        char command[10];
        sscanf(line, "%s", command);
        Liver oldLiver, newLiver;

        if (strcmp(command, "ADD") == 0) {
            Liver liver;
            sscanf(line, "%*s %s %s %s %d %d %d %c %lf",
                   liver.surname, liver.name, liver.patronymic,
                   &liver.day, &liver.month, &liver.year,
                   &liver.gender, &liver.income);
            insert(head, liver);
            push(stack, (Action){liver, 1});
            modifications++; 
        } else if (strcmp(command, "DELETE") == 0) {
            Liver liver;
            sscanf(line, "%*s %s %s %s %d %d %d %c %lf",
                   liver.surname, liver.name, liver.patronymic,
                   &liver.day, &liver.month, &liver.year,
                   &liver.gender, &liver.income);
            delete(head, liver);
            push(stack, (Action){liver, 2});
            modifications++;
        } else if (strcmp(command, "MODIFY") == 0) {
            sscanf(line, "%*s %s %s %s %d %d %d %c %lf %s %s %s %d %d %d %c %lf",
                   oldLiver.surname, oldLiver.name, oldLiver.patronymic,
                   &oldLiver.day, &oldLiver.month, &oldLiver.year,
                   &oldLiver.gender, &oldLiver.income,
                   newLiver.surname, newLiver.name, newLiver.patronymic,
                   &newLiver.day, &newLiver.month, &newLiver.year,
                   &newLiver.gender, &newLiver.income);
            Node* node = find(*head, oldLiver);
            if (node != NULL) {
                modify(node, newLiver);
                push(stack, (Action){oldLiver, 3});
                modifications++; 
            }
        } else if (strcmp(command, "UNDO") == 0) {
            int count = modifications / 2;
            undo(head, newLiver, stack, 1);
            modifications -= count;
        } else if (strcmp(command, "SAVE") == 0) {
            char filename[100];
            sscanf(line, "%*s %s", filename);
            save_to_file(*head, filename);
        } else if (strcmp(command, "PRINT") == 0) {
            print_liver(*head);
        } else if (strcmp(command, "FIND") == 0) {
            Liver liver = {"", "", "", 0, 0, 0, '\0', -1}; 
            sscanf(line, "%*s %s %s %s %d %d %d %c %lf",
                    liver.surname, liver.name, liver.patronymic,
                    &liver.day, &liver.month, &liver.year,
                    &liver.gender, &liver.income);

            Node* foundNode = find(*head, liver);
            if (foundNode != NULL) {
                printf("Найден житель:\n");
                printf("Фамилия: %s, Имя: %s, Отчество: %s, Дата рождения: %d.%d.%d, Пол: %c, Доход за месяц: %.2f\n\n",
                        foundNode->data.surname, foundNode->data.name, foundNode->data.patronymic,
                        foundNode->data.day, foundNode->data.month, foundNode->data.year,
                        foundNode->data.gender, foundNode->data.income);
            } else {
                printf("Житель не найден.\n\n");
            }
        }
    }
    free(line);
    fclose(file);
}

void free_liver(Node* head) {
    Node* current = head;
    Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3){
        printf("Неверное количество аргументов.\n");
        return 1;
    }
    
    Node* head = NULL;
    Stack stack;
    init_stack(&stack, 100);

    const char* input = argv[1];
    const char* command = argv[2];

    read_from_file(&head, input);
    commands(&head, &stack, command);
    free_liver(head);
    free_stack(&stack);

    return 0;
}