// steps_util_test.cpp
#include "steps_util_no_short_circuit.h"
#include <gtest/gtest.h>
#include <string>

using namespace wand_no_shortcircuit;

TEST(StepsUtilTest, StartProcessComposition)
{
    Start<int> start = []() { return 42; };
    Process<int, std::string> process = [](int x) { return std::to_string(x); };
    auto composed = start | process;
    EXPECT_EQ(composed(), "42");
}

TEST(StepUtilTest, Pipeline)
{
    Start<int> start = []() { return 10; };
    Process<int, int> process1 = [](int x) { return x + 5; };
    Process<int, int> process2 = [](int x) { return x * 2; };
    End<int> end = [](int result) { EXPECT_EQ(result, 30); }; // 10 -> 10 + 5 = 15 -> 15 * 2 = 30

    auto pipeline = start | process1 | process2 | end;
    pipeline(); // Trigger the pipeline
}
