#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct {
    double x;
    double y;
} Point;

double vector_product(Point P1, Point P2, Point P3) {
    return (P2.x - P1.x) * (P3.y - P1.y) - (P2.y - P1.y) * (P3.x - P1.x);
}

int convexity(int num_points, ...) {
    va_list args;
    va_start(args, num_points);

    Point* points = malloc(num_points * sizeof(Point));
    if (points == NULL) {
        printf("Ошибка выделения памяти.\n");
        return 0; 
    }

    for (int i = 0; i < num_points; i++) {
        points[i].x = va_arg(args, double);
        points[i].y = va_arg(args, double);
    }
    
    va_end(args);

    int sign = 0;

    for (int i = 0; i < num_points; i++) {
        double cp = vector_product(points[i], points[(i + 1) % num_points], points[(i + 2) % num_points]);
        if (cp != 0) {
            if (sign == 0) {
                if (cp > 0){
                    sign = 1;
                } else {
                    sign = -1;
                }
            } else if ((cp > 0 && sign < 0) || (cp < 0 && sign > 0)) {
                free(points);
                return 0; 
            }
        }
    }
    
    free(points);
    return 1; 
}

int main() {
    if (convexity(4, 0.0, -1.5, -4.0, -5.0, 4.0, 3.0, 0.0, 3.0)) {
        printf("Многоугольник является выпуклым.\n");
    } else {
        printf("Многоугольник не является выпуклым.\n");
    }

    return 0;
}
