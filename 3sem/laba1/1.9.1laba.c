#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

int char_int(const char *str) { // функция atoi (преобразование строки в целое число)
    int result = 0;
    int sign = 1; 
    if (*str == '-') {
        sign = -1;
        str++; 
    } else if (*str == '+') {
        str++; 
    }
    while (*str != '\0' && isdigit(*str)) {
        result = result * 10 + (*str - '0');
        str++;
    }
    return sign * result;
}

void fill_array(int arr[], int size, int a, int b) { // функция, которая заполняет массив случайными числами в заданном диапазоне (a, b]
    srand(time(NULL)); // инициализация генератора случайных чисел
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (b - a + 1) + a;
    }
}

void swap_max_min(int arr[], int size) { // поиск макс и мин, обмен местами
    int max_index = 0;
    int min_index = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] > arr[max_index]) {
            max_index = i;
        }
        if (arr[i] < arr[min_index]) {
            min_index = i;
        }
    }
    int temp = arr[max_index];
    arr[max_index] = arr[min_index];
    arr[min_index] = temp;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Неправильное количество аргументов!\n");
        return 1;
    }
    int a = char_int(argv[1]);
    int b = char_int(argv[2]);
    int arr1[10000];
    int size1 = char_int(argv[3]);
    if (size1 > 10000) {
        printf("Размер массива превышает допустимый!\n");
        return 1;
    }
    fill_array(arr1, size1, a, b);
    printf("Исходный массив:\n");
    for (int i = 0; i < size1; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");
    swap_max_min(arr1, size1);
    printf("Массив после замены максимального и минимального элементов:\n");
    for (int i = 0; i < size1; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");
    return 0;
}
