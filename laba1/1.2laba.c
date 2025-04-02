#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define MAX_PRIME 1000
#define MAX_INTERATION 47

long double factorial(int n) {
	long double res = 1.0;
	for (int i = 2; i <= n; i++) {
		res *= i;
	}
	return res;
}

void sieve_of_eratosthenes(int primes[], int *count) {
    int is_prime[MAX_PRIME + 1];
    for (int i = 0; i <= MAX_PRIME; i++) {
        is_prime[i] = 1; 
    }
    is_prime[0] = is_prime[1] = 0; 

    for (int i = 2; i * i <= MAX_PRIME; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i) {
                is_prime[j] = 0; 
            }
        }
    }

    *count = 0;
    for (int i = 2; i <= MAX_PRIME; i++) {
        if (is_prime[i]) {
            primes[(*count)++] = i; 
        }
    }
}

double e_limit(double epsilon) {
    double e = 1.0; 
    int n = 1;  
    double previous_e = 0.0; 
    while (fabs(e - previous_e) > epsilon) {
        previous_e = e;
        e = pow(1 + 1.0 / n, n);
        n++; 
    }
}

double e_series(double epsilon) {
    double sum = 1.0;
    double term = 1.0;
    int n = 1;
    while (fabs(term) > epsilon) {
        term /= n;
        sum += term;
        n++;
    }
    return sum;
}

double e_equation(double epsilon) {
    double x = 2.0; 
    double previous_x = 0.0;
    while (fabs(x - previous_x) > epsilon) {
        previous_x = x;
        x = x - (log(x) - 1) / (1.0 / x); 
    }
    return x;
}

double pi_limit(double epsilon) {
    double term;
    double lim = 0.0;
    int n = 0;

    do {
        term = pow(-1, n) / (2 * n + 1); 
        lim += term;
        n++;
    } while (fabs(M_PI - (lim * 4)) >= epsilon); 

    return lim * 4; 
}

double pi_series(double epsilon) {
    double sum = 0.0;
    int n = 1;
    double term = 1.0;
    while (fabs(term) > epsilon) {
        sum += term;
        n++;
        term = pow(-1, n - 1) / (2 * n - 1);
    }
    return 4 * sum;
}

double pi_equation(double epsilon) {
    double x = 3.0;
    double previous_x = 0.0;
    while (fabs(x - previous_x) > epsilon) {
        previous_x = x;
        x = x - (cos(x) + 1) / (-sin(x)); 
    }
    return x;
}

double ln2_limit(double epsilon) {
    int n = 1;
    double previous_lim = 0.0;
    double lim = n * (pow(2, 1.0 / n) - 1);
    while (fabs(lim - previous_lim) >= epsilon) {
        previous_lim = lim;
        n++;
        lim = n * (pow(2, 1.0 / n) - 1);
    }
  return lim;
}

double ln2_series(double epsilon) {
    double sum = 0.0;
    int n = 1;
    double term = 1.0;
    while (fabs(term) > epsilon) {
        sum += term;
        n++;
        term = pow(-1, n - 1) / n;
    }
    return sum;
}

double ln2_equation(double epsilon) {
    double x = 0.5; 
    double previous_x = 0.0;
    while (fabs(x - previous_x) > epsilon) {
        previous_x = x;
        x = x - (exp(x) - 2) / exp(x); 
    }
    return x;
}

double sqrt2_limit(double epsilon) {
    double lim = -0.5;
    double previous_lim = 0.0;
    int n = 0;
    while (fabs(lim - previous_lim) > epsilon) {
        previous_lim = lim;
        lim = lim - (pow(lim, 2) / 2.0 - 1);
        n++;
    }
    return lim;
}

double sqrt2_series(double epsilon) {
    double prod = 1.0;
    int k = 2;
    double term = pow(2, pow(2, -k));
    while (fabs(term - 1.0) >= epsilon) {
        prod *= term;
        k++;
        term = pow(2, pow(2, -k));
    }
    return prod;
}

double sqrt2_equation(double epsilon) {
    double x = 1.5;
    double previous_x = 0.0;
    while (fabs(x - previous_x) > epsilon) {
        previous_x = x;
        x = x - (pow(x, 2) - 2) / (2 * x); 
    }
    return x;
}

long double gamma_limit(double epsilon) {
    long double lim = 0.0;
	long double previous_gamma = 0.0;
	int m = 1;
	long double fact_m = 1;
	while (1) {
		long double sum = 0.0;
		long double fact_k = 1;
		for (int k = 1; k <= m; k++) {
			fact_k *= k;
			long double binom = fact_m / (fact_k * factorial(m - k));
			sum += binom * (k % 2 ? -1.0 : 1.0) * log(fact_k) / k;
		}
		if (m == 1) {
			previous_gamma = -100;
		} else {
			previous_gamma = lim;
		}
		lim = sum;
		if (m >= MAX_INTERATION) {
			return lim;
		}
		if (fabsl(lim - previous_gamma) < epsilon) {
			break;
		}
		m++;
		fact_m *= m;
	}
	return lim;
}


long double gamma_series(double epsilon) {
    long double sum = 0;
    long double term = 0;
    long double gamma_prev;
    int k = 2;
    while (1) {
        gamma_prev = sum;
        term = (1.0 / pow(floor(sqrt(k)), 2)) - (1.0 / k);
        sum += term;
        if (fabsl(term) < epsilon && term != 0) {
            break;
        }

        k++;
    }
    return sum - M_PI * M_PI / 6.0;
}

double gamma_equation(double epsilon) {
    int primes[MAX_PRIME];
    int prime_count = 0;
    sieve_of_eratosthenes(primes, &prime_count);
    double lim = 0.0;
    double ln_t = log((double)MAX_PRIME); 
    for (int i = 0; i < prime_count; i++) {
        if (primes[i] <= MAX_PRIME) {
            lim += log((double)(primes[i] - 1) / (double)primes[i]);
        }
    }
    lim = ln_t * exp(lim); 
    return lim; 
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    double epsilon = atof(argv[1]);
    if (epsilon <= 0) {
        printf("Epsilon должно быть положительным числом.\n");
        return 1;
    }

    printf("Число e (предел): %.4f\n", e_limit(epsilon));
    printf("Число e (ряд): %.4f\n", e_series(epsilon));
    printf("Число e (уравнение): %.4f\n", e_equation(epsilon));

    printf("Число pi (предел): %.4f\n", pi_limit(epsilon));
    printf("Число pi (ряд): %.4f\n", pi_series(epsilon));
    printf("Число pi (уравнение): %.4f\n", pi_equation(epsilon));

    printf("Число ln 2 (предел): %.4f\n", ln2_limit(epsilon));
    printf("Число ln 2 (ряд): %.4f\n", ln2_series(epsilon));
    printf("Число ln 2 (уравнение): %.4f\n", ln2_equation(epsilon));

    printf("Корень из двух (предел): %.4f\n", sqrt2_limit(epsilon));
    printf("Корень из двух (ряд): %.4f\n", sqrt2_series(epsilon));
    printf("Корень из двух (уравнение): %.4f\n", sqrt2_equation(epsilon));

    printf("Число Гамма (предел): %.4Lf\n", gamma_limit(epsilon));
    printf("Число Гамма (ряд): %.4Lf\n", gamma_series(epsilon));
    printf("Число Гамма (уравнение): %.4f\n", gamma_equation(epsilon));

    return 0;
}