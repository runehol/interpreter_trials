#include "bytecode.h"
#include <utility>
#include "interpreter.h"

namespace interpreter11
{

    struct CpuState
    {
        int32_t regs[16] = {0};
    };




    std::pair<int32_t, uint32_t> interpreter_run(uint8_t *program, int32_t param)
    {
        CpuState local_state;
        CpuState *state = &local_state;
        uint8_t *pc = program;
        state->regs[X0] = param;
        uint32_t cycle_count = 0;
        bool flags_zero = false;
        bool flags_negative = false;
        static void *dispatch_table[] =
            {
                &&op_return,
                &&op_add,
                &&op_sub,
                &&op_mov,
                &&op_movi,
                &&op_b,
                &&op_bnz
            };

        uint8_t opcode = *pc++;
        goto *dispatch_table[opcode];

    op_return:
        {
            cycle_count += 1;
            return std::make_pair(state->regs[X0], cycle_count);
        }

    op_add:
        {
            uint8_t reg = *pc++;
            uint8_t dest = reg>>4, src = reg & 0xf;

            int32_t v = state->regs[dest];
            v += state->regs[src];

            flags_zero = (v == 0);
            flags_negative = (v < 0);
            state->regs[dest] = v;
            cycle_count += 2;

            uint8_t opcode = *pc++;
            goto *dispatch_table[opcode];
        }

    op_sub:
        {
            uint8_t reg = *pc++;
            uint8_t dest = reg>>4, src = reg & 0xf;

            int32_t v = state->regs[dest];
            v -= state->regs[src];

            flags_zero = (v == 0);
            flags_negative = (v < 0);
            state->regs[dest] = v;
            cycle_count += 2;

            uint8_t opcode = *pc++;
            goto *dispatch_table[opcode];
        }

    op_mov:
        {
            uint8_t reg = *pc++;
            uint8_t dest = reg>>4, src = reg & 0xf;

            state->regs[dest] = state->regs[src];
            cycle_count += 2;

            uint8_t opcode = *pc++;
            goto *dispatch_table[opcode];
        }

    op_movi:
        {
            uint8_t reg = *pc++;
            uint8_t dest = reg>>4;

            int32_t imm = *(int32_t *)pc;
            pc += 4;

            state->regs[dest] = imm;
            cycle_count += 6;

            uint8_t opcode = *pc++;
            goto *dispatch_table[opcode];
        }

    op_b:
        {
            int8_t rel = *pc++;
            pc += rel;
            cycle_count += 2;

            uint8_t opcode = *pc++;
            goto *dispatch_table[opcode];
        }

    op_bnz:
        {
            int8_t rel = *pc++;
            if(!flags_zero)
            {
                pc += rel;
            }
            cycle_count += 2;

            uint8_t opcode = *pc++;
            goto *dispatch_table[opcode];
        }


    }


}
