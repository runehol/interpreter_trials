#include <benchmark/benchmark.h>
#include <iostream>

#include "bytecode.h"
#include "interpreter.h"


template<typename RunLoop>
void run(RunLoop run_loop)
{
    auto [res, cycles] = run_loop(fibonacci, 46);
    if(res != 1836311903)
    {
        throw std::runtime_error("Invalid result " + std::to_string(res));
    }
    if(cycles != 481)
    {
        throw std::runtime_error("Invalid cycle count " + std::to_string(cycles));

    }
}

static void interpreter1_fibonacci(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter1::interpreter_run);
    }
}
BENCHMARK(interpreter1_fibonacci);

static void interpreter2_fibonacci(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter2::interpreter_run);
    }
}
BENCHMARK(interpreter2_fibonacci);

static void interpreter3_fibonacci(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter3::interpreter_run);
    }
}
BENCHMARK(interpreter3_fibonacci);

static void interpreter4_fibonacci(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter4::interpreter_run);
    }
}
BENCHMARK(interpreter4_fibonacci);

BENCHMARK_MAIN();
