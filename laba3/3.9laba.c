#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char *word;
    int count;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Tree {
    Node *root;
} Tree;

char* strdup(const char *str) {
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

Node* create_node(const char *word) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    node->word = strdup(word);
    node->count = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Tree* create_tree() {
    Tree *tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    tree->root = NULL;
    return tree;
}

void insert_word(Tree *tree, const char *word) {
    if (tree->root == NULL) {
        tree->root = create_node(word);
    } else {
        Node *current = tree->root;
        while (1) {
            int cmp = strcmp(word, current->word);
            if (cmp == 0) {
                current->count++;
                break;
            } else if (cmp < 0) {
                if (current->left == NULL) {
                    current->left = create_node(word);
                    break;
                } else {
                    current = current->left;
                }
            } else {
                if (current->right == NULL) {
                    current->right = create_node(word);
                    break;
                } else {
                    current = current->right;
                }
            }
        }
    }
}

void destroy_node(Node *node) {
    if (node != NULL) {
        destroy_node(node->left);
        destroy_node(node->right);
        free(node->word); 
        free(node); 
    }
}

void destroy_tree(Tree *tree) {
    if (tree != NULL) {
        destroy_node(tree->root);
        free(tree); 
    }
}

int get_count(Tree *tree, const char *word) {
    Node *current = tree->root;
    while (current != NULL) {
        int cmp = strcmp(word, current->word);
        if (cmp == 0) {
            return current->count;
        } else if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return 0;
}

typedef struct {
    char *word;
    int count;
} WordCount;

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wc1 = (const WordCount *)a;
    const WordCount *wc2 = (const WordCount *)b;
    return wc2->count - wc1->count;
}

void collect_word_counts(Node *node, WordCount *wordCounts, int *count) {
    if (node == NULL) {
        return;
    }
    collect_word_counts(node->left, wordCounts, count);
    wordCounts[*count].word = strdup(node->word);
    wordCounts[*count].count = node->count;
    (*count)++;
    collect_word_counts(node->right, wordCounts, count);
}

void print_topn(Tree *tree, int n) {
    if (tree->root == NULL) {
        printf("Дерево пустое.\n");
        return;
    }

    int count = 0;
    WordCount wordCounts[1000];
    collect_word_counts(tree->root, wordCounts, &count);

    qsort(wordCounts, count, sizeof(WordCount), compare_word_counts);

    for (int i = 0; i < n && i < count; i++) {
        printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
    }

    for (int i = 0; i < count; i++) {
        free(wordCounts[i].word);
    }
}

void long_short(Node *node, char **longest, char **shortest) {
    if (node == NULL) {
        return;
    }
    if (*longest == NULL || strlen(node->word) > strlen(*longest)) {
        *longest = node->word;
    }
    if (*shortest == NULL || strlen(node->word) < strlen(*shortest)) {
        *shortest = node->word;
    }
    long_short(node->left, longest, shortest);
    long_short(node->right, longest, shortest);
}

void print_long_short(Tree *tree) {
    char *longest = NULL;
    char *shortest = NULL;
    long_short(tree->root, &longest, &shortest);
    if (longest != NULL && shortest != NULL) {
        printf("Самое длинное слово: %s\n", longest);
        printf("Самое короткое слово: %s\n", shortest);
    }
}

int deep(Node *node) {
    if (node == NULL) {
        return 0;
    }
    int left_depth = deep(node->left);
    int right_depth = deep(node->right);
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

int get_deep(Tree *tree) {
    return deep(tree->root);
}

void save_file(FILE *file, Node *node) {
    if (node == NULL) {
        fprintf(file, "NULL\n");
        return;
    }
    fprintf(file, "%s %d\n", node->word, node->count);
    save_file(file, node->left);
    save_file(file, node->right);
}

void save_tree(Tree *tree, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }
    save_file(file, tree->root);
    fclose(file);
}

Node* load_tree(FILE *file) {
    char word[100];
    int frequency;
    if (fscanf(file, "%s %d", word, &frequency) != 2) {
        return NULL;
    }
    Node *node = create_node(word);
    node->count = frequency;
    node->left = load_tree(file);
    node->right = load_tree(file);
    return node;
}

void load(Tree *tree, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }
    destroy_node(tree->root); 
    tree->root = load_tree(file);
    fclose(file);
}

char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла: %s\n", filename);
        return NULL;
    }

    int buffer_size = 1000;
    char *buffer = (char*)malloc(buffer_size);
    if (buffer == NULL) {
        printf("Ошибка выделения памяти.\n");
        fclose(file);
        return NULL;
    }

    int pos = 0;
    int c;

    while ((c = fgetc(file)) != EOF) {
        buffer[pos++] = (char)c;

        if (pos >= buffer_size - 1) {
            buffer_size *= 2;
            buffer = (char*)realloc(buffer, buffer_size);
            if (buffer == NULL) {
                printf("Ошибка выделения памяти.\n");
                fclose(file);
                return NULL;
            }
        }
    }

    buffer[pos] = '\0';

    fclose(file);
    return buffer;
}

void read_words(Tree *tree, const char *filename, const char *separators) {
    char *text = read_file(filename);
    if (text == NULL) {
        return;
    }

    char *word = strtok(text, separators);
    while (word != NULL) {
        if (strlen(word) > 0) {
            insert_word(tree, word);
        }
        word = strtok(NULL, separators);
    }

    free(text); 
}

void commands(Tree *tree, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    int buffer_size = 1000;
    char *command = (char*)malloc(buffer_size);
    if (command == NULL) {
        printf("Ошибка выделения памяти.\n");
        fclose(file);
        return;
    }

    while (fgets(command, buffer_size, file)) {
        command[strcspn(command, "\n")] = 0;

        while (strlen(command) == buffer_size - 1 && command[buffer_size - 2] != '\n') {
            buffer_size *= 2;
            command = (char*)realloc(command, buffer_size);
            if (command == NULL) {
                printf("Ошибка выделения памяти.\n");
                fclose(file);
                return;
            }
            if (!fgets(command + buffer_size / 2 - 1, buffer_size / 2 + 1, file)) {
                break;
            }
        }

        if (strncmp(command, "1 ", 2) == 0) {
            char word[100];
            sscanf(command + 2, "%s", word);
            int frequency = get_count(tree, word);
            printf("Сколько раз '%s' встречалось в файле: %d\n", word, frequency);
        } else if (strncmp(command, "2 ", 2) == 0) {
            printf("Наиболее часто встречающиеся слова в файле:\n");
            int n;
            sscanf(command + 2, "%d", &n);
            print_topn(tree, n);
        } else if (strcmp(command, "3") == 0) {
            print_long_short(tree);
        } else if (strcmp(command, "4") == 0) {
            int depth = get_deep(tree);
            printf("Глубина дерева: %d\n", depth);
        } else if (strncmp(command, "5 ", 2) == 0) {
            char saveFilename[100];
            sscanf(command + 2, "%s", saveFilename);
            save_tree(tree, saveFilename);
            printf("Дерево сохранено в %s\n", saveFilename);
        } else if (strncmp(command, "6 ", 2) == 0) {
            char loadFilename[100];
            char saveFilename[100];
            sscanf(command + 2, "%s %s", loadFilename, saveFilename);
            load(tree, loadFilename);
            save_tree(tree, saveFilename);
            printf("Дерево восстановлено из %s и сохранено в %s\n", loadFilename, saveFilename);
        } else {
            printf("Неизвестная команда: %s\n", command);
        }
    }
    free(command);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    const char *filename = argv[1];
    const char *separators = argv[2];
    const char *commands_file = argv[3];

    Tree *tree = create_tree();
    if (tree == NULL) {
        printf("Ошибка создания дерева.\n");
        return 1;
    }
    read_words(tree, filename, separators);
    commands(tree, commands_file);
    destroy_tree(tree);
    return 0;
}