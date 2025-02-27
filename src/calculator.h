#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK 100
#define MAX_INP 1024
#define MAX_NUMBER 2000000000
#define MIN_NUMBER -2000000000
#define FLOAT_PRECISION 1e-4

typedef struct {
    double data[MAX_STACK];
    int top;
} Stack;

void push(Stack* s, double value);

double pop(Stack* s);

double peek(Stack* s);

int precedence(char op);

double apply_op(double a, double b, char op, int is_float_mode);

int is_valid_char(char c);

double eval_expression(const char* expr, int is_float_mode);
