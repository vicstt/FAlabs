#include <iostream>
#include <cmath>

class Complex {
private:
    double real;
    double imaginary;

public:
    Complex (double r = 0.0, double i = 0.0) : real(r), imaginary(i) {}
    
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imaginary + other.imaginary);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imaginary - other.imaginary);
    }

    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imaginary * other.imaginary, 
                other.real * imaginary + real * other.imaginary);
    }

    Complex operator/(const Complex& other) const {
        double denominator = other.real * other.real + other.imaginary * other.imaginary;
        return Complex((real * other.real + imaginary * other.imaginary) / denominator,
                (imaginary * other.real - real * other.imaginary) / denominator);
    }

    double module() const {
        return std::sqrt(real * real + imaginary * imaginary);
    }

    double argument() const {
        return std::atan2(imaginary, real);
    }

    void output() const {
        if (imaginary > 0) {
            std::cout << real << " + " << imaginary << "i" << std::endl;
        }
        else {
            std::cout << real << " - " << -imaginary << "i" << std::endl;
        }
    }
};

int main() {
    Complex num1(5.0, 9.0);
    Complex num2(3.0, 4.0);

    Complex sum = num1 + num2;
    Complex difference = num1 - num2;
    Complex product = num1 * num2;
    Complex quotient = num1 / num2;

    std::cout << "Число 1: ";
    num1.output();
    std::cout << "Число 2: ";
    num2.output();
    std::cout << "Сумма: ";
    sum.output();
    std::cout << "Разность: ";
    difference.output();
    std::cout << "Произведение: ";
    product.output();
    std::cout << "Частное: ";
    quotient.output();
    std::cout << "Модуль числа 1: " << num1.module() << std::endl;
    std::cout << "Модуль числа 2: " << num2.module() << std::endl;
    std::cout << "Аргумент числа 1: " << num1.argument() << " радиан" << std::endl;
    std::cout << "Аргумент числа 2: " << num2.argument() << " радиан" << std::endl;
    
    return 0;
}