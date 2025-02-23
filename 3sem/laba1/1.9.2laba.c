#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int int_compare(const void *a, const void *b) { // функция сравнения для qsort
    return (*(int *)a - *(int *)b);
}

int binary_search(int *arr, int key, int size) {
    int low = 0; // левая граница
    int high = size - 1; //правая граница
    int mid = 0;
    
    while (low <= high) { // пока левая граница не станет правее правой
        mid = (low + high) / 2;
        
        if (arr[mid] == key) {
            return arr[mid];
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    if (mid > 0 && abs(arr[mid] - key) > abs(arr[mid - 1] - key)) {
        return arr[mid - 1];
    } else {
        return arr[mid];
    }
}

int main() {
    srand(time(NULL));
    
    int sizeA = rand() % 9991 + 10;
    int sizeB = rand() % 9991 + 10;
    
    int *A = (int*)malloc(sizeA * sizeof(int));
    int *B = (int*)malloc(sizeB * sizeof(int));
    int *C = (int*)malloc(sizeA * sizeof(int));
    
    printf("Массив A (размер - %d):\n", sizeA);
    for (int i = 0; i < sizeA; i++) {
        A[i] = rand() % 2001 - 1000;
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("Массив B (размер - %d):\n", sizeB);
    for (int i = 0; i < sizeB; i++) {
        B[i] = rand() % 2001 - 1000;
        printf("%d ", B[i]);
    }
    printf("\n");
    
    qsort(B, sizeB, sizeof(int), (int(*)(const void *, const void *))(void *)int_compare);

    printf("Массив C:\n");
    for (int i = 0; i < sizeA; i++) {
        C[i] = A[i] + binary_search(B, A[i], sizeB);
        printf("%d ", C[i]);
    }
    
    printf("\n");
    free(A);
    free(B);
    free(C);
    
    return 0;
}