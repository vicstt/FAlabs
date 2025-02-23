#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef struct {
    int n;        
    double *data;
} Vector;

typedef struct {
    Vector *vectors;  
    int count;        
} Result;

void free_vectors(Vector* v) {
    free(v->data);
    v->data = NULL;
    v->n = 0;
}

Vector vector_copy(Vector v) {
    Vector copy;
    copy.n = v.n;
    copy.data = malloc(v.n * sizeof(double));
    if (copy.data == NULL) {
        printf("Ошибка выделения памяти.\n");
    }
    for (int i = 0; i < v.n; i++) {
        copy.data[i] = v.data[i];
    }
    return copy;
}

double norm_inf(Vector v) {
    double max = fabs(v.data[0]);
    for (int i = 1; i < v.n; i++) {
        if (fabs(v.data[i]) > max) {
            max = fabs(v.data[i]);
        }
    }
    return max;
}

double norm_p(Vector v, double p) {
    double sum = 0.0;
    for (int i = 0; i < v.n; i++) {
        sum += pow(fabs(v.data[i]), p);
    }
    return pow(sum, 1.0 / p);
}

double norm_A(Vector v, double **A) {
    double result = 0.0;
    for (int i = 0; i < v.n; i++) {
        double temp = 0.0;
        for (int j = 0; j < v.n; j++) {
            temp += A[i][j] * v.data[j];
        }
        result += v.data[i] * temp;
    }
    return sqrt(result);
}

Result* find_max(int n, int num_vectors, double **A, int num_norms, double (*norm_functions[])(Vector, ...), double *max_norms, ...) {
    va_list args;
    va_start(args, max_norms);
    
    Vector *vectors = malloc(num_vectors * sizeof(Vector));
    if (vectors == NULL) {
        va_end(args);
        return NULL; 
    }
    for (int i = 0; i < num_vectors; i++) {
        vectors[i] = va_arg(args, Vector);
    }
    
    Result *results = malloc(num_norms * sizeof(Result));
    if (results == NULL) {
        free(vectors);
        va_end(args);
        return NULL; 
    }
    for (int i = 0; i < num_norms; i++) {
        results[i].vectors = malloc(num_vectors * sizeof(Vector));
        if (results[i].vectors == NULL) {
            for (int k = 0; k < i; k++) {
                free(results[k].vectors);
            }
            free(results);
            free(vectors);
            va_end(args);
            return NULL;
        }
        results[i].count = 0;
        max_norms[i] = -1.0;
    }
    
    for (int i = 0; i < num_vectors; i++) {
        for (int j = 0; j < num_norms; j++) {
            double norm_value;
            if (j == 1) { 
                norm_value = norm_functions[j](vectors[i], 2.0); 
            } else if (j == 2) { 
                norm_value = norm_functions[j](vectors[i], A); 
            } else { 
                norm_value = norm_functions[j](vectors[i]);
            }
            if (norm_value > max_norms[j]) {
                for (int k = 0; k < results[j].count; k++) {
                    free_vectors(&results[j].vectors[k]);
                }
                max_norms[j] = norm_value;
                results[j].vectors[0] = vector_copy(vectors[i]);
                results[j].count = 1;
            } else if (norm_value == max_norms[j]) {
                results[j].vectors[results[j].count] = vector_copy(vectors[i]);
                results[j].count++;
            }
        }
    }
    
    va_end(args);
    free(vectors);
    return results;
}

void free_result(Result *result, int num_norms) {
    for (int i = 0; i < num_norms; i++) {
        free(result[i].vectors);
    }
    free(result);
}

int main() {
    int n = 3; 
    int num_vectors = 2; 
    Vector *vectors = (Vector *)malloc(num_vectors * sizeof(Vector)); 
    
    vectors[0].n = n;
    vectors[0].data = (double *)malloc(n * sizeof(double));
    vectors[0].data[0] = -11.0;
    vectors[0].data[1] = -2.0;
    vectors[0].data[2] = 7.0;
    
    vectors[1].n = n;
    vectors[1].data = (double *)malloc(n * sizeof(double));
    vectors[1].data[0] = 1.0;
    vectors[1].data[1] = -2.0;
    vectors[1].data[2] = 11.0;

    double **A = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            A[i][j] = 1.0; 
        }
    }

    double (*norm_functions[])(Vector, ...) = {
        (double (*)(Vector, ...)) norm_inf,
        (double (*)(Vector, ...)) norm_p,
        (double (*)(Vector, ...)) norm_A
    };

    double max_norms[3];
    Result *results = find_max(n, num_vectors, A, 3, norm_functions, max_norms, vectors[0], vectors[1]);

    char *norm_names[] = {
        "inf",
        "p",
        "A"
    };

    for (int i = 0; i < 3; i++) {
        printf("Максимальная норма для нормы %s: %.2f\n", norm_names[i], max_norms[i]);
        printf("Векторы: ");
        for (int k = 0; k < results[i].count; k++) {
            for (int m = 0; m < results[i].vectors[k].n; m++) {
                printf("%.2f ", results[i].vectors[k].data[m]);
            }
            printf("\n");
        }
        printf("\n");
    }

    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < results[j].count; k++) {
            free_vectors(&results[j].vectors[k]);
        }
    }

    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    for (int i = 0; i < num_vectors; i++) {
        free(vectors[i].data);
    }
    free(vectors);

    free_result(results, 3);

    return 0;
}
