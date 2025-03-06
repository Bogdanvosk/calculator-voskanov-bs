#include "calculator.h"

void push(Stack* s, double value)
{
    if (s->top < MAX_STACK - 1) {
        s->data[++s->top] = value;
    }
}

double pop(Stack* s)
{
    return (s->top >= 0) ? s->data[s->top--] : 0;
}

double peek(Stack* s)
{
    return (s->top >= 0) ? s->data[s->top] : 0;
}

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

double apply_op(double a, double b, char op, int is_float_mode)
{
    if (op == '/' && (b > -FLOAT_PRECISION && b < FLOAT_PRECISION)) {
        fprintf(stderr, "Division by small number\n");
        exit(1);
    }
    double result;
    switch (op) {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        if (is_float_mode) {
            result = a / b;
        } else {
            result = (long long)(a / b); // Округление к 0
        }
        break;
    default:
        return 0;
    }
    if (!is_float_mode && (result < MIN_NUMBER || result > MAX_NUMBER)) {
        fprintf(stderr, "Intermediate result is out of range\n");
        exit(1);
    }
    return result;
}

int is_valid_char(char c)
{
    return isdigit(c) || c == '(' || c == ')' || c == '*' || c == '+' || c == '/' || c == '-' || c == '.' || isspace(c);
}

double eval_expression(const char* expr, int is_float_mode) {
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

        if (*expr == '-' && last_was_operator) {
            fprintf(stderr, "Incomplete expression\n");
            exit(1);
        }

        if (isdigit(*expr) || *expr == '.') {
            char* endptr;
            double value = strtod(expr, &endptr);
            if (!is_float_mode && (value < MIN_NUMBER || value > MAX_NUMBER)) {
                fprintf(stderr, "Number is out of range [%d - %d]\n", MIN_NUMBER, MAX_NUMBER);
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
                if (values.top < 1) {
                    fprintf(stderr, "Incomplete expression\n");
                    exit(1);
                }
                double b = pop(&values);
                double a = pop(&values);
                char op = (char)pop(&operators);
                push(&values, apply_op(a, b, op, is_float_mode));
            }
            if (operators.top >= 0 && peek(&operators) == '(') {
                pop(&operators);
            }
            open_parentheses--;
            last_was_operator = 0;
        } else {
            if (last_was_operator) {
                fprintf(stderr, "Incomplete expression\n");
                exit(1);
            }
            while (operators.top >= 0 && peek(&operators) != '(' &&
                   precedence(peek(&operators)) >= precedence(*expr)) {
                if (values.top < 1) {
                    fprintf(stderr, "Incomplete expression\n");
                    exit(1);
                }
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
        if (values.top < 1) {
            fprintf(stderr, "Incomplete expression\n");
            exit(1);
        }
        double b = pop(&values);
        double a = pop(&values);
        char op = (char)pop(&operators);
        push(&values, apply_op(a, b, op, is_float_mode));
    }

    if (values.top != 0) {
        fprintf(stderr, "Invalid expression\n");
        exit(1);
    }

    return pop(&values);
}
