#include "bytecode.h"
#include <utility>
#include "interpreter.h"

namespace interpreter4
{

    struct CpuState
    {
        int32_t regs[16] = {0};
        bool zero_flag = false;
        bool negative_flag = false;
        uint32_t cycle_count = 0;
    };


    struct ReturnException
    {
    };

    uint8_t *op_return(CpuState *state, uint8_t *pc)
    {
        state->cycle_count += 1;
        throw ReturnException();
    }

    uint8_t *op_add(CpuState *state, uint8_t *pc)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        int32_t v = state->regs[dest];
        v += state->regs[src];

        state->zero_flag = (v == 0);
        state->negative_flag = (v < 0);
        state->regs[dest] = v;
        state->cycle_count += 2;
        return pc;
    }

    uint8_t *op_sub(CpuState *state, uint8_t *pc)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        int32_t v = state->regs[dest];
        v -= state->regs[src];

        state->zero_flag = (v == 0);
        state->negative_flag = (v < 0);
        state->regs[dest] = v;
        state->cycle_count += 2;
        return pc;
    }

    uint8_t *op_mov(CpuState *state, uint8_t *pc)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        state->regs[dest] = state->regs[src];
        state->cycle_count += 2;
        return pc;

    }

    uint8_t *op_movi(CpuState *state, uint8_t *pc)
    {
        uint8_t reg = *pc++;
        uint8_t dest = reg>>4;

        int32_t imm = *(int32_t *)pc;
        pc += 4;

        state->regs[dest] = imm;
        state->cycle_count += 6;
        return pc;

    }

    uint8_t *op_b(CpuState *state, uint8_t *pc)
    {
        int8_t rel = *pc++;
        pc += rel;
        state->cycle_count += 2;
        return pc;
    }

    uint8_t *op_bnz(CpuState *state, uint8_t *pc)
    {
        int8_t rel = *pc++;
        if(!state->zero_flag)
        {
            pc += rel;
        }
        state->cycle_count += 2;
        return pc;
    }

    uint8_t *(*dispatch_table[])(CpuState *state, uint8_t *pc) =
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
        try
        {
            while(true)
            {
                uint8_t opcode = *pc++;
                pc = dispatch_table[opcode](state, pc);


            }


        } catch(ReturnException ex)
        {
            return std::make_pair(state->regs[X0], state->cycle_count);
        }

    }


}
