#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int str_len(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int str_cmp(const char *str1, const char *str2, int n) {
    for (int i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
        if (str1[i] == '\0') {
            return 0; 
        }
    }
    return 0;
}

void str_cpy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void search_substring_in_files(const char *substring, int num_files, ...) {
    va_list files;
    va_start(files, num_files);

    for (int f = 0; f < num_files; f++) {
        const char *filename = va_arg(files, const char *);
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("Не удалось открыть файл %s.\n", filename);
            continue;
        }

        char buffer[1000];
        char previous_buffer[1000]; 
        int line_number = 0;
        int substring_len = str_len(substring);
        int last_char_pos = 0;

        while (fgets(buffer, 1000, file)) {
            line_number++;

            // проверка на стыке строк
            int i = 0;
            for (int i = 0; previous_buffer[i] != '\0' && i < substring_len; i++) {
                if (i < str_len(previous_buffer)) {
                    if (str_cmp(&previous_buffer[str_len(previous_buffer) - i], substring + i, substring_len - i) == 0 && str_cmp(&buffer[i - 1], substring + substring_len - i, i) == 0) { 
                        printf("Файл: %s, Номер строки: %d, Позиция: %d\n", filename, line_number - 1, last_char_pos + i); 
                        break; 
                    }
                }
            }

            for (int i = 0; buffer[i] != '\0'; i++) {
                if (str_cmp(&buffer[i], substring, substring_len) == 0) {
                    printf("Файл: %s, Номер строки: %d, Позиция: %d\n", filename, line_number, i + last_char_pos + 1);
                }
            }

            last_char_pos += str_len(buffer);
            str_cpy(previous_buffer, buffer); 
        }

        fclose(file);
    }
    va_end(files);
}

int main() {
    search_substring_in_files("ch ch", 3, "file1.txt", "file2.txt", "file3.txt");
    return 0;
}
