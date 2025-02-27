#include "calculator.h"

int main(int argc, char* argv[])
{
    int is_float_mode = 0;
    if (argc > 1 && strcmp(argv[1], "--float") == 0) {
        is_float_mode = 1;
    }

    char expr[MAX_INP];
    if (fgets(expr, MAX_INP, stdin) != NULL) {
        expr[strcspn(expr, "\n")] = 0;

        if (strlen(expr) >= MAX_INP) {
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
