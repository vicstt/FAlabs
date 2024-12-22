#include <iostream>
#include <cstring>
#include <cmath>

class logical_values_array {
private:
    unsigned int value;

public:
    logical_values_array(unsigned int val = 0) : value(val) {}

    unsigned int get_value() const {
        return value;
    }

    logical_values_array inversion() const {
        return logical_values_array(~value);
    }

    logical_values_array conjunction(const logical_values_array& other) const {
        return logical_values_array(value & other.value);
    }

    logical_values_array disjunction(const logical_values_array& other) const {
        return logical_values_array(value | other.value);
    }

    logical_values_array implication(const logical_values_array& other) const {
        return logical_values_array(inversion().disjunction(other));
    }

    logical_values_array coimplication(const logical_values_array& other) const {
        return logical_values_array(implication(other).conjunction(other.implication(*this)));
    }

    logical_values_array addition_modulo_2(const logical_values_array& other) const {
        return logical_values_array(value ^ other.value);
    }

    logical_values_array equivalence(const logical_values_array& other) const {
        return logical_values_array(conjunction(other).disjunction(inversion().conjunction(other.inversion())));
    }

    logical_values_array pierce_arrow(const logical_values_array& other) const {
        return logical_values_array(disjunction(other).inversion());
    }

    logical_values_array schaeffers_stroke(const logical_values_array& other) const {
        return logical_values_array(conjunction(other).inversion());
    }

    static bool equals(const logical_values_array& a, const logical_values_array& b) {
        return a.value == b.value;
    }

    bool get_bit(int position) const {
        return (value >> position) & 1;
    }

   void to_binary_string(char* buffer) const {
        const int bits = sizeof(value) * 8; 
        buffer[bits] = '\0';

        for (int i = 0; i < bits; ++i) {
            buffer[bits - 1 - i] = (get_bit(i) ? '1' : '0');
        }
    }
};

int main() {
    logical_values_array a(5); 
    logical_values_array b(3);

    char binaryString[33];
    a.to_binary_string(binaryString);
    std::cout << "Двоичное представление a: " << binaryString << std::endl;

    b.to_binary_string(binaryString);
    std::cout << "Двоичное представление b: " << binaryString << std::endl;


    std::cout << "Инверсия a: " << a.inversion().get_value() << std::endl;
    std::cout << "Конъюнкция a и b: " << a.conjunction(b).get_value() << std::endl;
    std::cout << "Дизъюнкция a и b: " << a.disjunction(b).get_value() << std::endl;
    std::cout << "Импликация a и b: " << a.implication(b).get_value() << std::endl;
    std::cout << "Коимпликация a и b: " << a.coimplication(b).get_value() << std::endl;
    std::cout << "Сложение по модулю 2 a и b: " << a.addition_modulo_2(b).get_value() << std::endl;
    std::cout << "Эквивалентность a и b: " << a.equivalence(b).get_value() << std::endl;
    std::cout << "Стрелка Пирса a и b: " << a.pierce_arrow(b).get_value() << std::endl;
    std::cout << "Штрих Шеффера a и b: " << a.schaeffers_stroke(b).get_value() << std::endl;
    std::cout << "Эквивалентность a и b: " << (logical_values_array::equals(a, b) ? "true" : "false") << std::endl;
    std::cout << "Значение бита a по его поизиции: " << a.get_bit(1) << std::endl;

    return 0;
}
