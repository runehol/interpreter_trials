#include "bytecode.h"
#include <utility>
#include "interpreter.h"

namespace interpreter7
{

    struct Flags
    {
        unsigned int zero:16;
        unsigned int negative:16;
    };

    struct CpuState
    {
        int32_t regs[16] = {0};
        uint32_t final_cycle_count = 0;
    };



#define PARAMS CpuState *state, uint8_t *pc, Flags flags, uint32_t cycle_count
#define ARGS state, pc, flags, cycle_count

    extern void (*dispatch_table[])(PARAMS);

    void op_return(PARAMS)
    {
        cycle_count += 1;
        state->final_cycle_count = cycle_count;
        return;
    }

    void op_add(PARAMS)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        int32_t v = state->regs[dest];
        v += state->regs[src];

        flags.zero = (v == 0);
        flags.negative = (v < 0);
        state->regs[dest] = v;
        cycle_count += 2;

        uint8_t opcode = *pc++;
        dispatch_table[opcode](ARGS);
    }

    void op_sub(PARAMS)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        int32_t v = state->regs[dest];
        v -= state->regs[src];

        flags.zero = (v == 0);
        flags.negative = (v < 0);
        state->regs[dest] = v;
        cycle_count += 2;

        uint8_t opcode = *pc++;
        dispatch_table[opcode](ARGS);
    }

    void op_mov(PARAMS)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        state->regs[dest] = state->regs[src];
        cycle_count += 2;

        uint8_t opcode = *pc++;
        dispatch_table[opcode](ARGS);
    }

    void op_movi(PARAMS)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4;

        int32_t imm = *(int32_t *)pc;
        pc += 4;

        state->regs[dest] = imm;
        cycle_count += 6;

        uint8_t opcode = *pc++;
        dispatch_table[opcode](ARGS);
    }

    void op_b(PARAMS)
    {
        int8_t rel = *pc++;
        pc += rel;
        cycle_count += 2;

        uint8_t opcode = *pc++;
        dispatch_table[opcode](ARGS);
    }

    void op_bnz(PARAMS)
    {
        int8_t rel = *pc++;
        if(!flags.zero)
        {
            pc += rel;
        }
        cycle_count += 2;

        uint8_t opcode = *pc++;
        dispatch_table[opcode](ARGS);
    }

    void (*dispatch_table[])(PARAMS) =
    {
        op_return,
        op_add,
        op_sub,
        op_mov,
        op_movi,
        op_b,
        op_bnz
    };

    std::pair<int32_t, uint32_t> interpreter_run(uint8_t *program, int32_t param)
    {
        CpuState local_state;
        CpuState *state = &local_state;
        uint8_t *pc = program;
        state->regs[X0] = param;
        uint32_t cycle_count = 0;
        Flags flags = {0, 0};

        uint8_t opcode = *pc++;
        dispatch_table[opcode](ARGS);

        return std::make_pair(state->regs[X0], state->final_cycle_count);
    }


}
