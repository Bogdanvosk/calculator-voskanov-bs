#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define MAX_STACK 100
#define MAX_INPUT 1024
#define MAX_NUMBER 2000000000
#define MIN_NUMBER -2000000000
#define FLOAT_PRECISION 1e-4

typedef struct {
    double data[MAX_STACK];
    int top;
} Stack;

void push(Stack *s, double value) {
    if (s->top < MAX_STACK - 1) {
        s->data[++s->top] = value;
    }
}

double pop(Stack *s) {
    return (s->top >= 0) ? s->data[s->top--] : 0;
}

double peek(Stack *s) {
    return (s->top >= 0) ? s->data[s->top] : 0;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double apply_op(double a, double b, char op, int is_float_mode) {
    if (op == '/' && (b > -FLOAT_PRECISION && b < FLOAT_PRECISION)) {
        fprintf(stderr, "Division by small number\n");
        exit(1);
    }
    double result;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if (is_float_mode) {
                result = a / b;
            } else {
                result = (a / b);
                if (result < 0 && ((long long)a % (long long)b) != 0) {
                    result -= 1;
                }
            }
            break;
        default: return 0;
    }
    if (!is_float_mode && (result < MIN_NUMBER || result > MAX_NUMBER)) {
        fprintf(stderr, "Intermediate result is out of range\n");
        exit(1);
    }
    return result;
}

int is_valid_char(char c) {
    return isdigit(c) || c == '(' || c == ')' || c == '*' || c == '+' || c == '/' || c == '-' || c == '.' || isspace(c);
}

double eval_expression(const char *expr, int is_float_mode) {
    Stack values = { .top = -1 };
    Stack operators = { .top = -1 };
    int last_was_operator = 1;
    int open_parentheses = 0;

    while (*expr) {
        if (!is_valid_char(*expr)) {
            fprintf(stderr, "Invalid character\n");
            exit(1);
        }
        if (isspace(*expr)) {
            expr++;
            continue;
        }
        if (isdigit(*expr) || *expr == '.') {
            char *endptr;
            double value = strtod(expr, &endptr);
            if (!is_float_mode && (value < 0 || value > MAX_NUMBER)) {
                fprintf(stderr, "Number is out of range [0 - 2*10^9]\n");
                exit(1);
            }
            push(&values, value);
            expr = endptr;
            last_was_operator = 0;
            continue;
        }
        if (*expr == '(') {
            push(&operators, *expr);
            open_parentheses++;
            last_was_operator = 1;
        } else if (*expr == ')') {
            if (open_parentheses <= 0) {
                fprintf(stderr, "Incomplete expression (parentheses)\n");
                exit(1);
            }
            while (operators.top >= 0 && peek(&operators) != '(') {
                double b = pop(&values);
                double a = pop(&values);
                char op = (char)pop(&operators);
                push(&values, apply_op(a, b, op, is_float_mode));
            }
            pop(&operators);
            open_parentheses--;
            last_was_operator = 0;
        } else {
            if (last_was_operator) {
                fprintf(stderr, "Incomplete expression\n");
                exit(1);
            }
            while (operators.top >= 0 && precedence(peek(&operators)) >= precedence(*expr)) {
                double b = pop(&values);
                double a = pop(&values);
                char op = (char)pop(&operators);
                push(&values, apply_op(a, b, op, is_float_mode));
            }
            push(&operators, *expr);
            last_was_operator = 1;
        }
        expr++;
    }
    if (last_was_operator || open_parentheses > 0) {
        fprintf(stderr, "Incomplete expression\n");
        exit(1);
    }
    while (operators.top >= 0) {
        double b = pop(&values);
        double a = pop(&values);
        char op = (char)pop(&operators);
        push(&values, apply_op(a, b, op, is_float_mode));
    }
    return pop(&values);
}

int main(int argc, char *argv[]) {
    int is_float_mode = 0;
    if (argc > 1 && strcmp(argv[1], "--float") == 0) {
        is_float_mode = 1;
    }

    char expr[MAX_INPUT];
    if (fgets(expr, MAX_INPUT, stdin) != NULL) {
        expr[strcspn(expr, "\n")] = 0;

        if (strlen(expr) >= MAX_INPUT) {
            fprintf(stderr, "Input data size must be less than 1KiB\n");
            return 1;
        }
        double result = eval_expression(expr, is_float_mode);
        if (is_float_mode) {
            printf("%.4f\n", result);
        } else {
            printf("%d\n", (int)result);
        }
    }
    return 0;
}
