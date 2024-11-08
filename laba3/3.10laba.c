#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char value; 
    struct Node** child; 
    int child_count;
    int level; 
} Node;

Node* create_node(char value, int level) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    new_node->child = NULL;
    new_node->child_count = 0;
    new_node->level = level; 
    return new_node;
}

void add_child(Node* parent, Node* child) {
    parent->child = (Node**)realloc(parent->child, (parent->child_count + 1) * sizeof(Node*));
    parent->child[parent->child_count] = child;
    parent->child_count++;
}

Node* build_tree(const char* expression, int* index, int level) {
    while (expression[*index] == ' '){
        (*index)++; 
    }

    if (expression[*index] == '\0') return NULL;

    Node* node = create_node(expression[*index], level); 
    (*index)++;

    while (expression[*index] != '\0') {
        while (expression[*index] == ' '){
            (*index)++; 
        }
        if (expression[*index] == '(') {
            (*index)++; 
            Node* child = build_tree(expression, index, level + 1); 
            if (child != NULL) {
                add_child(node, child);
            }
            if (expression[*index] == ')') {
                (*index)++; 
                break;
            }
        } else if (expression[*index] == ')') {
            (*index)++;
            level--;
            break; 
        } else if (expression[*index] == ',') {
            (*index)++; 
        } else {
            Node* child = create_node(expression[*index], level);
            add_child(node, child);
            (*index)++; 
        }
    }

    return node;
}

void print_tree(Node* node, FILE* file, int level) {
    if (node == NULL) return;
    for (int i = 0; i < node->level; i++) {
        fprintf(file, "\t"); 
    }
    fprintf(file, "%c\n", node->value); 

    for (int i = 0; i < node->child_count; i++) {
        print_tree(node->child[i], file, node->child[i]->level);
    }
}

void free_tree(Node* node) {
    if (node == NULL) return;
    for (int i = 0; i < node->child_count; i++) {
        free_tree(node->child[i]);
    }
    free(node->child);
    free(node);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    char *inputfile = argv[1];
    char *outputfile = argv[2];

    FILE* input = fopen(inputfile, "r");
    FILE* output = fopen(outputfile, "w");

    if (input == NULL || output == NULL) {
        printf("Ошибка при открытии файла.\n");
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), input)) {
        int index = 0;
        Node* root = build_tree(line, &index, 0); 
        fprintf(output, "Дерево для выражения: %s\n", line);
        print_tree(root, output, 0);
        fprintf(output, "\n");
        free_tree(root);
    }
    fclose(input);
    fclose(output);
    return 0;
}