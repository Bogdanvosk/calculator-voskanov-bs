#include <cstdlib>
#include <gtest/gtest.h>

extern "C" {
#include "../../src/calculator.h"
}

// Тест для проверки добавления нескольких элементов в стек
TEST(StackTest, PushElements)
{
    Stack s;
    s.top = -1;

    push(&s, 10.5);
    push(&s, 20.3);
    push(&s, 30.7);

    EXPECT_EQ(s.top, 2);

    EXPECT_EQ(s.data[0], 10.5);
    EXPECT_EQ(s.data[1], 20.3);
    EXPECT_EQ(s.data[2], 30.7);
}

// Тест для проверки извлечения нескольких элементов из стека
TEST(StackTest, PopElements)
{
    Stack s;
    s.top = -1; // Инициализация стека

    // Добавляем несколько элементов
    push(&s, 10.5);
    push(&s, 20.3);
    push(&s, 30.7);

    // Извлекаем элементы из стека
    double value1 = pop(&s);
    double value2 = pop(&s);
    double value3 = pop(&s);

    // Проверяем, что элементы извлечены в правильном порядке
    EXPECT_EQ(value1, 30.7); // Последний добавленный элемент извлекается первым
    EXPECT_EQ(value2, 20.3);
    EXPECT_EQ(value3, 10.5);

    // Проверяем, что стек пуст
    EXPECT_EQ(s.top, -1);
}

TEST(StackTest, PeekElements)
{
    Stack s;
    s.top = -1; // Инициализация стека

    // Добавляем несколько элементов
    push(&s, 10.5);
    push(&s, 20.3);
    push(&s, 30.7);

    // Получаем верхний элемент стека
    double value = peek(&s);

    // Проверяем, что значение корректно
    EXPECT_EQ(value, 30.7); // Проверяем, что возвращено правильное значение
    EXPECT_EQ(s.top, 2); // Проверяем, что стек не изменился (top остался прежним)
}

// Параметризованный тест для функции precedence
TEST(PrecedenceTest, AllOperators)
{
    EXPECT_EQ(precedence('+'), 1);
    EXPECT_EQ(precedence('-'), 1);
    EXPECT_EQ(precedence('*'), 2);
    EXPECT_EQ(precedence('/'), 2);

    EXPECT_EQ(precedence('('), 0);
    EXPECT_EQ(precedence(')'), 0);
}

// Тест для операции сложения
TEST(ApplyOpTest, Addition)
{
    EXPECT_DOUBLE_EQ(apply_op(5.3, 3.2, '+', 1), 8.5); // Режим с плавающей точкой
    EXPECT_DOUBLE_EQ(apply_op(5.0, 3.0, '+', 0), 8.0); // Целочисленный режим
}

// Тест для операции вычитания
TEST(ApplyOpTest, Subtraction)
{
    EXPECT_DOUBLE_EQ(apply_op(5.3, 3.2, '-', 1), 2.1); // Режим с плавающей точкой
    EXPECT_DOUBLE_EQ(apply_op(5.0, 3.0, '-', 0), 2.0); // Целочисленный режим
}

// Тест для операции умножения
TEST(ApplyOpTest, Multiplication)
{
    EXPECT_DOUBLE_EQ(apply_op(5.5, 3.0, '*', 1), 16.5); // Режим с плавающей точкой
    EXPECT_DOUBLE_EQ(apply_op(5.0, 3.0, '*', 0), 15.0); // Целочисленный режим
}

// Тест для операции деления (режим с плавающей точкой)
TEST(ApplyOpTest, DivisionFloatMode)
{
    EXPECT_DOUBLE_EQ(apply_op(10.6, 2.0, '/', 1), 5.3); // Нормальное деление
    EXPECT_DOUBLE_EQ(apply_op(10.0, 3.0, '/', 1), 10.0 / 3.0); // Деление с дробным результатом
}

// Тест для операции деления (целочисленный режим)
TEST(ApplyOpTest, DivisionIntegerMode)
{
    EXPECT_DOUBLE_EQ(apply_op(10.0, 2.0, '/', 0), 5.0); // Нормальное деление
    EXPECT_DOUBLE_EQ(apply_op(10.0, 3.0, '/', 0), 3.0); // Округление к 0
    EXPECT_DOUBLE_EQ(apply_op(-10.0, 3.0, '/', 0), -3.0); // Округление к 0 для отрицательных чисел
}

// Тест для деления на ноль (или близкое к нулю значение)
TEST(ApplyOpTest, DivisionByZero)
{
    // Проверяем, что функция завершает программу при делении на ноль
    EXPECT_EXIT(apply_op(10.0, 0.0, '/', 1), ::testing::ExitedWithCode(1), "Division by small number");
    EXPECT_EXIT(apply_op(10.0, 0.000001, '/', 1), ::testing::ExitedWithCode(1), "Division by small number");
}

// Тест для выхода за пределы допустимого диапазона (целочисленный режим)
TEST(ApplyOpTest, OutOfRange)
{
    // Проверяем, что функция завершает программу при выходе за пределы диапазона
    EXPECT_EXIT(apply_op(MAX_NUMBER, 2.0, '*', 0), ::testing::ExitedWithCode(1), "Intermediate result is out of range");
    EXPECT_EXIT(apply_op(MIN_NUMBER, 2.0, '*', 0), ::testing::ExitedWithCode(1), "Intermediate result is out of range");
    EXPECT_EXIT(apply_op(MAX_NUMBER, MAX_NUMBER, '*', 0), ::testing::ExitedWithCode(1), "Intermediate result is out of range");
    EXPECT_EXIT(apply_op(MIN_NUMBER, MIN_NUMBER, '*', 0), ::testing::ExitedWithCode(1), "Intermediate result is out of range");
}

// Тест для некорректного оператора
TEST(ApplyOpTest, InvalidOperator)
{
    EXPECT_DOUBLE_EQ(apply_op(5.0, 3.0, '^', 1), 0.0); // Некорректный оператор
    EXPECT_DOUBLE_EQ(apply_op(5.0, 3.0, '^', 0), 0.0); // Некорректный оператор
}

// Тест для цифр
TEST(IsValidCharTest, Digits)
{
    for (char c = '0'; c <= '9'; ++c) {
        EXPECT_TRUE(is_valid_char(c)) << "Digit " << c << " should be valid";
    }
}

// Тест для операторов
TEST(IsValidCharTest, Operators)
{
    EXPECT_TRUE(is_valid_char('+'));
    EXPECT_TRUE(is_valid_char('-'));
    EXPECT_TRUE(is_valid_char('*'));
    EXPECT_TRUE(is_valid_char('/'));
}

// Тест для скобок
TEST(IsValidCharTest, Parentheses)
{
    EXPECT_TRUE(is_valid_char('('));
    EXPECT_TRUE(is_valid_char(')'));
}

// Тест для точки
TEST(IsValidCharTest, DecimalPoint)
{
    EXPECT_TRUE(is_valid_char('.'));
}

// Тест для пробельных символов
TEST(IsValidCharTest, Whitespace)
{
    EXPECT_TRUE(is_valid_char(' '));
    EXPECT_TRUE(is_valid_char('\t'));
    EXPECT_TRUE(is_valid_char('\n'));
    EXPECT_TRUE(is_valid_char('\r'));
}

// Тест для недопустимых символов
TEST(IsValidCharTest, InvalidCharacters)
{
    EXPECT_FALSE(is_valid_char('a'));
    EXPECT_FALSE(is_valid_char('@'));
    EXPECT_FALSE(is_valid_char('#'));
    EXPECT_FALSE(is_valid_char('$'));
}

// Тесты для корректных выражений (целочисленный режим)
TEST(EvalExpressionTest, ValidIntegerExpressions)
{
    EXPECT_DOUBLE_EQ(eval_expression("2 + 3 * 4", 0), 14.0); // Приоритет операторов
    EXPECT_DOUBLE_EQ(eval_expression("(2 + 3) * 4", 0), 20.0); // Скобки
    EXPECT_DOUBLE_EQ(eval_expression("10 / 3", 0), 3.0); // Округление к 0
    EXPECT_DOUBLE_EQ(eval_expression("2000000000", 0), 2e9); // Граничное значение
}

// Тесты для корректных выражений (режим с плавающей точкой)
TEST(EvalExpressionTest, ValidFloatExpressions)
{
    EXPECT_NEAR(eval_expression("2.5 + 3.5", 1), 6.0, 1e-6); // Сложение дробных
    EXPECT_NEAR(eval_expression("10.0 / 3.0", 1), 3.333333, 1e-6); // Деление
    EXPECT_NEAR(eval_expression("(1.5 * 2) - 0.5", 1), 2.5, 1e-6); // Скобки
}

// Тесты на ошибки (некорректные символы, синтаксис, переполнение)
TEST(EvalExpressionTest, ErrorHandling)
{
    // Недопустимый символ
    EXPECT_EXIT(eval_expression("2 + a", 0), ::testing::ExitedWithCode(1), "Invalid character");

    // Незакрытая скобка
    EXPECT_EXIT(eval_expression("(2 + 3 * 4", 0), ::testing::ExitedWithCode(1), "Incomplete expression");

    // Два оператора подряд
    EXPECT_EXIT(eval_expression("2 ++ 3", 0), ::testing::ExitedWithCode(1), "Incomplete expression");

    // Переполнение в целочисленном режиме
    EXPECT_EXIT(eval_expression("2000000000 * 2", 0), ::testing::ExitedWithCode(1), "out of range");
}

// Тест пробелов и формата
TEST(EvalExpressionTest, WhitespaceHandling)
{
    EXPECT_DOUBLE_EQ(eval_expression("  2   +   3  ", 0), 5.0); // Пробелы
    EXPECT_DOUBLE_EQ(eval_expression("\t2\n+\t3", 0), 5.0); // Табуляция и перевод строки
}

TEST(EvalExpressionTest, EdgeCases)
{
    // Минимальное значение
    EXPECT_DOUBLE_EQ(eval_expression("-2000000000", 0), -2e9);

    // Выход за диапазон
    EXPECT_EXIT(eval_expression("-2000000001", 0), ::testing::ExitedWithCode(1), "out of range");

    // Корректное выражение с отрицательным числом
    EXPECT_DOUBLE_EQ(eval_expression("-2000000000 + 1", 0), -1999999999.0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
