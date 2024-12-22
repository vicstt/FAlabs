#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define HASHSIZE 128
#define INITIAL_BUFFER_SIZE 1000

typedef struct Macro {
    char* name;
    char* value;
    struct Macro* next;
} Macro;

typedef struct Hashtable {
    Macro** table;
    int size;
} Hashtable;

char* strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }
    char *dup = (char*)malloc(strlen(str) + 1);
    if (dup == NULL) {
        return NULL;
    }
    strcpy(dup, str);
    return dup;
}

Hashtable* create_table(int size) {
    Hashtable* hash_table = (Hashtable*)malloc(sizeof(Hashtable));
    if (hash_table == NULL) {
        printf("Ошибка выделения памяти.");
        return NULL;
    }
    hash_table->size = size;
    hash_table->table = (Macro**)malloc(sizeof(Macro*) * size);
    if (hash_table->table == NULL) {
        printf("Ошибка выделения памяти.");
        free(hash_table);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        hash_table->table[i] = NULL;
    }
    return hash_table;
}

unsigned int hash(const char* key) {
    unsigned long value = 0;
    while (*key) {
        if (isdigit(*key)) {
            value = value * 62 + (*key - '0');
        } else if (islower(*key)) {
            value = value * 62 + (*key - 'a' + 10);
        } else if (isupper(*key)) {
            value = value * 62 + (*key - 'A' + 36);
        }
        key++;
    }
    return value % HASHSIZE;
}

void insert_macro(Hashtable* hash_table, const char* name, const char* value) {
    int index = hash(name);
    Macro* macro = (Macro*)malloc(sizeof(Macro));
    if (macro == NULL) {
        printf("Ошибка выделения памяти.");
        return;
    }
    macro->name = strdup(name);
    macro->value = strdup(value);
    macro->next = hash_table->table[index];
    hash_table->table[index] = macro;
}

Hashtable* rehash(Hashtable* old_table) {
    int new_size = old_table->size * 2;
    Hashtable* new_table = create_table(new_size);
    
    unsigned int* cached_hashes = (unsigned int*)malloc(sizeof(unsigned int) * old_table->size);
    if (cached_hashes == NULL) {
        printf("Ошибка выделения памяти.");
        return NULL;
    }

    for (int i = 0; i < old_table->size; i++) {
        Macro* macro = old_table->table[i];
        while (macro) {
            cached_hashes[i] = hash(macro->name);
            insert_macro(new_table, macro->name, macro->value);
            macro = macro->next;
        }
    }

    free(cached_hashes);
    
    for (int i = 0; i < old_table->size; i++) {
        Macro* current = old_table->table[i];
        while (current) {
            Macro* temp = current;
            current = current->next;
            free(temp->name);
            free(temp->value);
            free(temp);
        }
    }
    free(old_table->table);
    free(old_table);
    
    return new_table;
}

void check(Hashtable** hash_table) {
    int min_length = INT_MAX;
    int max_length = 0;

    for (int i = 0; i < (*hash_table)->size; i++) {
        int length = 0;
        Macro* macro = (*hash_table)->table[i];
        while (macro) {
            length++;
            macro = macro->next;
        }
        if (length > 0) {
            if (length < min_length) min_length = length;
            if (length > max_length) max_length = length;
        }
    }

    if (max_length >= 2 * min_length) {
        *hash_table = rehash(*hash_table);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    char* input_file = argv[1];

    FILE* input = fopen(input_file, "r");
    if (input == NULL) {
        printf("Ошибка при открытии файла.\n");
        return 1;
    }

    Hashtable* hash_table = create_table(HASHSIZE);
    char* buffer = (char*)malloc(INITIAL_BUFFER_SIZE);
    if (buffer == NULL) {
         printf("Ошибка выделения памяти.");
        return 1;
    }
    int buffer_size = INITIAL_BUFFER_SIZE;
    int count_line = 0;
    char* lines[1000];

    while (fgets(buffer, buffer_size, input)) {
        if (strncmp(buffer, "#define", 7) == 0) {
            char* name = strtok(buffer + 8, " ");
            char* value = strtok(NULL, "\n");
            insert_macro(hash_table, name, value);
            check(&hash_table);
        } else {
            lines[count_line] = strdup(buffer);
            count_line++;
        }
    }

    fclose(input);
    free(buffer);

    for (int i = 0; i < count_line; i++) {
        char* line = lines[i];
        char* res = (char*)malloc(strlen(line) + 1);
        if (res == NULL) {
            printf("Ошибка выделения памяти.");
            return 1;
        }
        strcpy(res, line);
        for (int j = 0; j < HASHSIZE; j++) {
            Macro* current = hash_table->table[j];
            while (current) {
                char* pos;
                while ((pos = strstr(res, current->name)) != NULL) {
                    char* new_line = (char*)malloc(strlen(res) + strlen(current->value) - strlen(current->name) + 1);
                    if (new_line == NULL) {
                        printf("Ошибка выделения памяти.");
                        return 1;
                    }
                    strncpy(new_line, res, pos - res);
                    new_line[pos - res] = '\0';
                    strcat(new_line, current->value);
                    strcat(new_line, pos + strlen(current->name));
                    free(res);
                    res = new_line;
                }
                current = current->next;
            }
        }
        printf("%s", res);
        free(res);
        free(line);
    }

    for (int i = 0; i < HASHSIZE; i++) {
        Macro* current = hash_table->table[i];
        while (current) {
            Macro* temp = current;
            current = current->next;
            free(temp->name);
            free(temp->value);
            free(temp);
        }
    }
    
    printf("\n");
    free(hash_table->table);
    free(hash_table);

    return 0;
}