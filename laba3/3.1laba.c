#include <stdio.h>
#include <stdlib.h>

int sum(int a, int b) {
    while (b != 0) {
        int carry = a & b; 
        a = a ^ b;         
        b = carry << 1; 
    }
    return a;
}

char int_char(int digit) {
    if (digit < 10) {
        return '0' + digit;  
    } else {
        return 'A' + (digit - 10); 
    }
}

void to_base(int number, int r, char *res){
    int index = 0;
    
    if (number == 0){
        res[sum(index, 1)] = '\0';
        res[index] = '0';
        return;
    }

    int base = 1 << r;
    while (number > 0) {
        int digit = number & (sum(base, - 1)); 
        number >>= r;
        res[index] = int_char(digit);
        index = sum(index, 1);
    }

    res[index] = '\0';

    int i = 0;
    while (i < index >> 1){
        char temp = res[i];
        res[i] = res[sum(index, - sum(1, i))];
        res[sum(index, - sum(1, i))] = temp;
        i = sum(i, 1);
    }

}

int main(int argc, char **argv){
    if (argc != 2){
        printf("Неверное количество аргументов.\n");
        return 1;
    }
    char res[1000];
    int num = atoi(argv[1]);
    for (int r = 1; r <= 5; r++){
        to_base(num, r, res);
        printf("Число %d в системе счисления с основанием %d: %s\n", num, 1 << r, res);
    }
    return 0;
}