#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef struct {
    int n;        
    double *data;
} Vector;

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

void find_max(int n, int num_vectors, double **A, int num_norms, double (*norm_functions[])(Vector, ...), ...) {
    va_list args;
    va_start(args, norm_functions);
    
    Vector *vectors = malloc(num_vectors * sizeof(Vector));
    for (int i = 0; i < num_vectors; i++) {
        vectors[i] = va_arg(args, Vector);
    }
    
    double *max_norms = malloc(num_norms * sizeof(double));
    Vector **max_vectors = malloc(num_norms * sizeof(Vector *));
    
    for (int i = 0; i < num_norms; i++) {
        max_norms[i] = -1.0;
        max_vectors[i] = NULL;
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
                max_norms[j] = norm_value;
                max_vectors[j] = &vectors[i];
            }
        }
    }
    
    char *norm_names[] = {
        "inf",
        "p",
        "A"
    };

    for (int i = 0; i < num_norms; i++) {
        printf("Максимальная норма для нормы %s: %.2f\n", norm_names[i], max_norms[i]);
        printf("Вектор: ");
        for (int k = 0; k < max_vectors[i]->n; k++) {
            printf("%.2f ", max_vectors[i]->data[k]);
        }
        printf("\n");
    }
    
    va_end(args);
    free(vectors);
    free(max_vectors);
    free(max_norms);
}
int main() {
    int n = 3; 
    int num_vectors = 2; 
    Vector *vectors = (Vector *)malloc(num_vectors * sizeof(Vector)); 
    
    vectors[0].n = n;
    vectors[0].data = (double *)malloc(n * sizeof(double));
    vectors[0].data[0] = 1.0;
    vectors[0].data[1] = -2.0;
    vectors[0].data[2] = -7.0;
    
    vectors[1].n = n;
    vectors[1].data = (double *)malloc(n * sizeof(double));
    vectors[1].data[0] = 1.0;
    vectors[1].data[1] = -2.0;
    vectors[1].data[2] = -7.0;

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
    
    find_max(n, num_vectors, A, 3, norm_functions, vectors[0], vectors[1]);

    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    for (int i = 0; i < num_vectors; i++) {
        free(vectors[i].data);
    }
    
    return 0;
}
