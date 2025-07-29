#pragma once

#include <functional>

namespace wand_no_shortcircuit
{
template<typename TStartOut>
using Start = std::function<TStartOut()>;

template<typename TProcessIn, typename TProcessOut>
using Process = std::function<TProcessOut(TProcessIn)>;

template<typename TEndIn>
using End = std::function<void(TEndIn)>;

using Action = std::function<void(void)>;

/** Start | process -0> Start
 ** Composes a Start with a Process, returning a new Start that applies the Process to the output of the Start.
 ** Example:
 **  Start<int> start = []() { return 42; };
 **  Process<int, std::string> process = [](int x) { return std::to_string(x); };
 **  auto composed = start | process;
 **  composed(); // Returns "42"
 */
template<typename TInput, typename TOutput>
Start<TOutput> operator|(Start<TInput> start, Process<TInput, TOutput> process)
{
    return [=]() -> TOutput
    {
        return process(std::forward<TInput>(start()));
    };
}

/**
 * process | process -> process
 * Composes two Processes, returning a new Process that applies the first Process and then the second Process to its input.
 * Example:
 * Process<int, int> processOne = [](int x) { return x + 1; };
 * Process<int, int> processTwo = [](int x) { return x * 2; };
 * auto composed = processOne | processTwo;
 * composed(3); // Returns 8 (i.e., (3 + 1) * 2)
 */
template<typename TInput, typename TMiddle, typename TOutput>
Process<TInput, TOutput> operator|(Process<TInput, TMiddle> processOne,
        Process<TMiddle, TOutput> processTwo)
{
    return [=](TInput input) -> TOutput
    {
        return processTwo(std::forward<TMiddle>(processOne(std::forward<TInput>(input))));
    };
}

/**
 * process | end -> end
 * Composes a Process with an End, returning a new End that applies the Process to its input and then calls the End with the output.
 * Example:
 * Process<int, std::string> process = [](int x) { return std::to_string(x); };
 * End<std::string> end = [](std::string result) { std::cout << result; };
 * auto composed = process | end;
 * composed(42); // Outputs "42"
 */
template<typename TInput, typename TOutput>
End<TOutput> operator|(Process<TInput, TOutput> process, End<TOutput> end)
{
    return [=](TInput input) {
        TOutput output = process(std::forward<TInput>(input));
        end(std::forward<TOutput>(output));
    };
}

/**
 * start | end -> Action
 * Composes a Start with an End, returning an Action that applies the Start and then calls the End with the output.
 * Example:
 * Start<int> start = []() { return 42; };
 * End<int> end = [](int result) { std::cout << result; };
 * auto composed = start | end;
 * composed(); // Outputs "42"
 */
template <typename TStartOut>
Action operator|(Start<TStartOut> start, End<TStartOut> end)
{
        return [=]() {
            end(std::forward<TStartOut>(start()));
        };
}

} // namespace wand