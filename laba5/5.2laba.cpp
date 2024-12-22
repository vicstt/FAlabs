#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Encoder {
private:
    std::vector<unsigned char> key;
    
public:
    Encoder(const std::vector<unsigned char>& k) : key(k) {}

    void encode(const std::string& input, const std::string& output, bool encrypt) {
        std::ifstream in(input.c_str(), std::ios::binary);
        std::ofstream out(output.c_str(), std::ios::binary);

        if (!in.is_open() || !out.is_open()) {
            std::cerr << "Не удалось открыть файл." << std::endl;
        }

        std::vector<unsigned char> s(256);
        std::vector<unsigned char> t(256);

        for (int i = 0; i < 256; ++i) {
            s[i] = static_cast<unsigned char>(i);
            t[i] = key[i % key.size()];
        }

        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + s[i] + t[i]) % 256;
            std::swap(s[i], s[j]);
        }

        int i = 0;
        unsigned char byte;
        while (in.read(reinterpret_cast<char*>(&byte), 1)) {
            i = (i + 1) % 256;
            j = (j + s[i]) % 256;
            std::swap(s[i], s[j]);
            unsigned char k = s[(s[i] + s[j]) % 256];
            if (encrypt) {
                byte ^= k;
            } else {
                byte ^= k;
            }
            out.write(reinterpret_cast<char*>(&byte), 1);
        }
    }

    void set_key(const std::vector<unsigned char>& k) {
        key = k;
    }
};

int main() {
    unsigned char key_data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    std::vector<unsigned char> key(key_data, key_data + sizeof(key_data) / sizeof(key_data[0]));

    Encoder encoder(key);

    encoder.encode("5.2input.txt", "5.2encryption.txt", true);
    std::cout << "Файл успешно зашифрован." << std::endl;

    encoder.encode("5.2encryption.txt", "5.2encryption.txt", false);
    std::cout << "Файл успешно расшифрован." << std::endl;


    unsigned char new_key_data[] = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    std::vector<unsigned char> new_key(new_key_data, new_key_data + sizeof(new_key_data) / sizeof(new_key_data[0]));

    encoder.set_key(new_key);

    encoder.encode("5.2input.txt", "5.2encryption.txt", true);
    std::cout << "Файл успешно зашифрован с новым ключом." << std::endl;

    encoder.encode("5.2encryption.txt", "5.2decryption.txt", false);
    std::cout << "Файл успешно расшифрован." << std::endl;

    return 0;
}