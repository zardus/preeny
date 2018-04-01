#define _GNU_SOURCE

#include <inttypes.h>

#include "logging.h"

/**
 * Copyright (c) 2017 elttam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifdef __x86_64__
#define canary_t     uint64_t
#define INSN_READ    "movq %%fs:0x28, %0;"
#define FMT          "Found canary: %#lx\n"

#elif __i386__
#define canary_t     uint32_t
#define INSN_READ    "movl %%gs:0x14, %0;"
#define FMT          "Found canary: %#x\n"
#endif

canary_t read_canary()
{
    canary_t val = 0;

    __asm__(INSN_READ
        : "=r"(val)
        :
        :);

    return val;
}

//
// Dump stack cookie on startup
//
__attribute__((constructor)) void preeny_cookie_dump()
{
    preeny_logging_init(); //This haven't been called when we get here so env flags are not respected unless called manually
    preeny_info(FMT, read_canary());
}
