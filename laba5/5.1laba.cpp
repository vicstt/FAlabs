#include <iostream>
#include <bitset>

class binary_int {
private:
    int value;

public:
    binary_int(int val = 0) : value(val) {}

    int sum(int a, int b) const {
        while (b != 0) {
            int carry = a & b; 
            a = a ^ b;         
            b = carry << 1; 
        }
        return a;
    }

    int difference(int a, int b) const {
        while (b != 0) {
		    int borrow = (~a) & b;
		    a = a ^ b;
		    b = borrow << 1;
	    }
	    return a;
    }

    int product(int a, int b) const {
	    int result = 0;
	    while (b != 0) {
		    if (b & 1) {
			    result = sum(result, a);
		    }
		    a <<= 1;
		    b >>= 1;
	    }
	    return result;
    }

    binary_int operator-() const {
        return binary_int(sum(~value, 1));
    }

    binary_int& operator++() {  // префиксный
        value = sum(value, 1);
        return *this;
    }

    binary_int operator++(int) {   
        binary_int temp = *this;
        ++(*this);
        return temp;
    }

    binary_int& operator--() {
        value = difference(value, 1);
        return *this;
    }

    binary_int operator--(int) {
        binary_int temp = *this;
        --(*this);
        return temp;
    }

    binary_int& operator+=(const binary_int& other) {
        value = sum(value, other.value);
        return *this;
    }

    binary_int operator+(const binary_int& other) const {
        binary_int result = *this;
        result += other;
        return result;
    }

    binary_int& operator-=(const binary_int& other) {  
        value = difference(value, other.value);
        return *this;
    }

    binary_int operator-(const binary_int& other) const {
        binary_int result = *this;
        result -= other;
        return result;
    }

    binary_int& operator*=(const binary_int& other) {
        value = product(value, other.value);
        return *this;
    }

    binary_int operator*(const binary_int& other) const {
        binary_int result = *this;
        result *= other;
        return result;
    }  

    binary_int& operator<<=(int shift) {
        value = value << shift;
        return *this;
    }

    binary_int& operator>>=(int shift) {
        value = value >> shift;
        return *this;
    }

    binary_int operator<<(int shift) const {
        binary_int result = *this;
        result <<= shift;
        return result;
    }

    binary_int operator>>(int shift) const {
        binary_int result = *this;
        result >>= shift;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const binary_int& bi) {
        os << std::bitset<sizeof(int) * 8>(bi.value);
        return os;
    }

        std::pair<binary_int, binary_int> split() const {
        int half_bits = sizeof(int) * 8 / 2;
        int upper = (value >> half_bits) << half_bits; 
        int lower = value & ((1 << half_bits) - 1); 
        return {binary_int(upper), binary_int(lower)};
    }
};

int main() {
    binary_int a(7); 
    binary_int b(3); 

    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;

	std::cout << "-a = " << -a << std::endl << std::endl;

    binary_int c = a + b;
    std::cout << "a + b: " << c << std::endl;

    binary_int d = a - b;
    std::cout << "a - b: " << d << std::endl;

	std::cout << "a++ = " << a++  << ", a = " << a << std::endl;
	std::cout << "++a = " << ++a << " , a = " << a << std::endl << std::endl;


	std::cout << "a-- = " << a--  << ", a = " << a << std::endl;
	std::cout << "--a = " << --a << " , a = " << a << std::endl << std::endl;

    binary_int e = a * b;
    std::cout << "a * b: " << e << std::endl;

	a *= b;
	std::cout << "a *= b: " << a << std::endl << std::endl;

	binary_int f = a << 2;
	std::cout << "a << 2 = " << f << std::endl;
	a <<= 2;
	std::cout << "a <<= 2: " << a << std::endl << std::endl;

	binary_int g = a >> 2;
	std::cout << "a >> 2 = " << g << std::endl;
	a >>= 2;
	std::cout << "a >>= 2: " << a << std::endl << std::endl;


	binary_int k(545654647);
	std::cout << "k = " << k << std::endl;
	std::pair<binary_int, binary_int> result = k.split();
	std::cout << "Половина старших битов: " << result.first << std::endl;
    std::cout << "Половина младших битов: " << result.second << std::endl;

	return 0;
}