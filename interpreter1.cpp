#include "bytecode.h"
#include <utility>
#include "interpreter.h"

namespace interpreter1
{

    struct CpuState
    {
        int32_t regs[16] = {0};
        bool zero_flag = false;
        bool negative_flag = false;
        uint8_t *pc = nullptr;
        uint32_t cycle_count = 0;
        bool exit = false;
    };

    CpuState global_state;

    void op_return()
    {
        global_state.exit = true;
    }

    void op_add()
    {
        uint8_t reg = *global_state.pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        int32_t v = global_state.regs[dest];
        v += global_state.regs[src];

        global_state.zero_flag = (v == 0);
        global_state.negative_flag = (v < 0);
        global_state.regs[dest] = v;

    }

    void op_sub()
    {
        uint8_t reg = *global_state.pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        int32_t v = global_state.regs[dest];
        v -= global_state.regs[src];

        global_state.zero_flag = (v == 0);
        global_state.negative_flag = (v < 0);
        global_state.regs[dest] = v;
    }

    void op_mov()
    {
        uint8_t reg = *global_state.pc++;
        uint8_t dest = reg>>4, src = reg & 0xf;

        global_state.regs[dest] = global_state.regs[src];

    }

    void op_movi()
    {
        uint8_t reg = *global_state.pc++;
        uint8_t dest = reg>>4;

        int32_t imm = *(int32_t *)global_state.pc;
        global_state.pc += 4;

        global_state.regs[dest] = imm;

    }

    void op_b()
    {
        int8_t rel = *global_state.pc++;
        global_state.pc += rel;
    }

    void op_bnz()
    {
        int8_t rel = *global_state.pc++;
        if(!global_state.zero_flag)
        {
            global_state.pc += rel;
        }
    }

    void (*dispatch_table[])() =
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
        global_state.pc = program;
        global_state.regs[X0] = param;
        global_state.cycle_count = 0;
        global_state.exit = false;

        while(!global_state.exit)
        {
            uint8_t opcode = *global_state.pc++;
            global_state.cycle_count += cycle_table[opcode];
            dispatch_table[opcode]();
        }

        return std::make_pair(global_state.regs[X0], global_state.cycle_count);
    }


}
