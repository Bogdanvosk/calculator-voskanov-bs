#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char* str;

void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

int calc_number() {
    int num = 0;
    while (isdigit(*str)) {
        num = num * 10 + (*str - '0');
        str++;
    }
    return num;
}

int calc_expression();

int calc_mult_div() {
    int num = calc_expression(); // Получаем первое число или результат выражения в скобках

    while (*str == '*' || *str == '/') {
        char op = *str;
        str++;
        int next_num = calc_expression();
        if (op == '*') {
            num *= next_num;
        } else if (op == '/') {
            num /= next_num;
        }
    }

    return num;
}

int calc_add_sub() {
    int res = calc_mult_div();

    while (*str == '+' || *str == '-') {
        char op = *str;
        str++;
        int next_num = calc_mult_div();
        if (op == '+') {
            res += next_num;
        } else if (op == '-') {
            res -= next_num;
        }
    }

    return res;
}

int calc_expression() {
    int res = 0;

    if (*str == '(') {
        str++;
        res = calc_add_sub();
        str++;
    } else {
        res = calc_number();
    }

    return res;
}

int main() {
    char s[100];
    fgets(s, sizeof(s), stdin);

    remove_spaces(s);
    str = s;

    int result = calc_add_sub();
    printf("%d", result);

    return 0;
}
