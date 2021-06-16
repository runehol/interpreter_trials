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

static void interpreter1_baseline(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter1::interpreter_run);
    }
}
BENCHMARK(interpreter1_baseline);

static void interpreter2_state_as_parameters(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter2::interpreter_run);
    }
}
BENCHMARK(interpreter2_state_as_parameters);

static void interpreter3_cycle_count_in_op_funs(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter3::interpreter_run);
    }
}
BENCHMARK(interpreter3_cycle_count_in_op_funs);

static void interpreter4_pc_in_registers(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter4::interpreter_run);
    }
}
BENCHMARK(interpreter4_pc_in_registers);

static void interpreter5_pc_flags_in_registers(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter5::interpreter_run);
    }
}
BENCHMARK(interpreter5_pc_flags_in_registers);

static void interpreter6_pc_flags_cycle_count_in_registers(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter6::interpreter_run);
    }
}
BENCHMARK(interpreter6_pc_flags_cycle_count_in_registers);

static void interpreter7_tail_calls(benchmark::State& state) {
    for (auto _ : state)
    {
        run(interpreter7::interpreter_run);
    }
}
BENCHMARK(interpreter7_tail_calls);

BENCHMARK_MAIN();
