# Calculator Project

## What has been done?
This project is a command-line calculator implemented in C. It supports basic arithmetic operations, handles operator precedence and parentheses, and provides both integer and floating-point calculation modes.
Unit tests are implemented using Google Test.

## How to run/use it?
### Build the project
To compile the application and unit tests, run:
```sh
make
```

### Running the calculator
#### Integer mode:
```sh
make run-int
```
#### Floating-point mode:
```sh
make run-float
```

After running, input an arithmetic expression. The program will evaluate and print the result.
Example:
```
Input: 10 + 5 * 2
Output: 20
```

```
Input: (10 - 3) / 2
Output: 3
```

### Running unit tests
To execute unit tests:
```sh
make run-unit-test
```

## How it's made?
The project is built using C and consists of:
- `src/main.c` — Handles user input and execution.
- `src/calculator.c` — Implements expression evaluation using a stack.
- `src/calculator.h` — Function prototypes and macro definitions.

### Key Features
- Supports `+`, `-`, `*`, `/` operations.
- Handles parentheses for proper operation precedence.
- Integer mode (range: -2,000,000,000 to 2,000,000,000).
- Floating-point mode (precision: `1e-4`).
- Error handling for invalid expressions, division by zero, and out-of-range values.

### Development tools
- **Makefile** for compilation and running tests.
- **Google Test** for unit testing.
- **Clang-Format** for code formatting (`make format`).
