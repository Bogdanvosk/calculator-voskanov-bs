#include <gtest/gtest.h>

extern "C" {
#include "../../src/calculator.h"
}

// Тест для функции push
TEST(StackTest, PushTest)
{
    Stack s;
    double a = 15;
    double b = 30;
    double c = 45;

    s.top = -1;

    push(&s, a);
    push(&s, b);
    push(&s, c);
    EXPECT_EQ(s.top, c) << "Error: push in stack";
    pop(&s);
    EXPECT_EQ(s.top, b) << "Error: push in stack";
    pop(&lss);
    EXPECT_EQ(s.top, a) << "Error: push in stack";
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
