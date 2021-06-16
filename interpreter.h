#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <utility>
#include <cstdint>

namespace interpreter1
{
    std::pair<int32_t, uint32_t> interpreter1_run(uint8_t *program, int32_t param);
}

#endif
