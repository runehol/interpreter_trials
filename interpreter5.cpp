#include "bytecode.h"
#include <utility>
#include "interpreter.h"

namespace interpreter5
{

    struct Flags
    {
        unsigned int zero:16;
        unsigned int negative:16;
    };

    struct CpuState
    {
        int32_t regs[16] = {0};
        uint32_t cycle_count = 0;
    };


    struct ReturnException
    {
    };


    struct ReturnVal
    {
        uint8_t *pc;
        Flags flags;
    };


#define PARAMS CpuState *state, uint8_t *pc, Flags flags
#define ARGS state, pc, flags
#define RETURN_VAL {pc, flags}

    ReturnVal op_return(PARAMS)
    {
        state->cycle_count += 1;
        throw ReturnException();
    }

    ReturnVal op_add(PARAMS)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        int32_t v = state->regs[dest];
        v += state->regs[src];

        flags.zero = (v == 0);
        flags.negative = (v < 0);
        state->regs[dest] = v;
        state->cycle_count += 2;
        return RETURN_VAL;
    }

    ReturnVal op_sub(PARAMS)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        int32_t v = state->regs[dest];
        v -= state->regs[src];

        flags.zero = (v == 0);
        flags.negative = (v < 0);
        state->regs[dest] = v;
        state->cycle_count += 2;
        return RETURN_VAL;
    }

    ReturnVal op_mov(PARAMS)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        state->regs[dest] = state->regs[src];
        state->cycle_count += 2;
        return RETURN_VAL;

    }

    ReturnVal op_movi(PARAMS)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4;

        int32_t imm = *(int32_t *)pc;
        pc += 4;

        state->regs[dest] = imm;
        state->cycle_count += 6;
        return RETURN_VAL;

    }

    ReturnVal op_b(PARAMS)
    {
        int8_t rel = *pc++;
        pc += rel;
        state->cycle_count += 2;
        return RETURN_VAL;
    }

    ReturnVal op_bnz(PARAMS)
    {
        int8_t rel = *pc++;
        if(!flags.zero)
        {
            pc += rel;
        }
        state->cycle_count += 2;
        return RETURN_VAL;
    }

    ReturnVal (*dispatch_table[])(PARAMS) =
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
        state->cycle_count = 0;
        Flags flags = {0, 0};
        try
        {
            while(true)
            {
                uint8_t opcode = *pc++;
                ReturnVal ret = dispatch_table[opcode](ARGS);
                pc = ret.pc;
                flags = ret.flags;
            }


        } catch(ReturnException ex)
        {
            return std::make_pair(state->regs[X0], state->cycle_count);
        }

    }


}
