#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STACK 100

typedef struct {
    int data[MAX_STACK];
    int top;
} Stack;

void push(Stack *s, int value) {
    if (s->top < MAX_STACK - 1) {
        s->data[++s->top] = value;
    }
}

int pop(Stack *s) {
    return (s->top >= 0) ? s->data[s->top--] : 0;
}

int peek(Stack *s) {
    return (s->top >= 0) ? s->data[s->top] : 0;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int apply_op(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

int eval_expression(const char *expr) {
    Stack values;
    Stack operators;

    values.top = -1;
    operators.top = -1;

    while (*expr) {
        if (isspace(*expr)) {
            expr++;
            continue;
        }
        if (isdigit(*expr)) {
            int value = strtol(expr, (char **)&expr, 10);
            push(&values, value);
            continue;
        }
        if (*expr == '(') {
            push(&operators, *expr);
        } else if (*expr == ')') {
            while (operators.top >= 0 && peek(&operators) != '(') {
                int b = pop(&values);
                int a = pop(&values);
                char op = (char)pop(&operators);
                push(&values, apply_op(a, b, op));
            }
            pop(&operators);
        } else {
            while (operators.top >= 0 && precedence(peek(&operators)) >= precedence(*expr)) {
                int b = pop(&values);
                int a = pop(&values);
                char op = (char)pop(&operators);
                push(&values, apply_op(a, b, op));
            }
            push(&operators, *expr);
        }
        expr++;
    }

    while (operators.top >= 0) {
        int b = pop(&values);
        int a = pop(&values);
        char op = (char)pop(&operators);
        push(&values, apply_op(a, b, op));
    }

    return pop(&values);
}

int main() {
    char expr[256];
    fgets(expr, sizeof(expr), stdin);

    int result = eval_expression(expr);
    printf("%d\n", result);

    return 0;
}
