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
    if(cycles != 480)
    {
        throw std::runtime_error("Invalid cycle count " + std::to_string(cycles));

    }
}

// Define another benchmark
static void interpreter1_fibonacci(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter1::interpreter1_run);
    }
}
BENCHMARK(interpreter1_fibonacci);

BENCHMARK_MAIN();
