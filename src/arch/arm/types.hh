/*
 * Copyright (c) 2007-2008 The Florida State University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Stephen Hines
 */

#ifndef __ARCH_ARM_TYPES_HH__
#define __ARCH_ARM_TYPES_HH__

#include "base/bitunion.hh"
#include "base/types.hh"

namespace ArmISA
{
    typedef uint32_t MachInst;

    BitUnion64(ExtMachInst)
        // Made up bitfields that make life easier.
        Bitfield<33>     sevenAndFour;
        Bitfield<32>     isMisc;

        // All the different types of opcode fields.
        Bitfield<27, 25> encoding;
        Bitfield<25>     useImm;
        Bitfield<24, 21> opcode;
        Bitfield<24, 20> mediaOpcode;
        Bitfield<24>     opcode24;
        Bitfield<23, 20> opcode23_20;
        Bitfield<23, 21> opcode23_21;
        Bitfield<20>     opcode20;
        Bitfield<22>     opcode22;
        Bitfield<19>     opcode19;
        Bitfield<18>     opcode18;
        Bitfield<15, 12> opcode15_12;
        Bitfield<15>     opcode15;
        Bitfield<7,  4>  miscOpcode;
        Bitfield<7,5>    opc2;
        Bitfield<7>      opcode7;
        Bitfield<4>      opcode4;

        Bitfield<31, 28> condCode;
        Bitfield<20>     sField;
        Bitfield<19, 16> rn;
        Bitfield<15, 12> rd;
        Bitfield<11, 7>  shiftSize;
        Bitfield<6,  5>  shift;
        Bitfield<3,  0>  rm;

        Bitfield<11, 8>  rs;

        SubBitUnion(puswl, 24, 20)
            Bitfield<24> prepost;
            Bitfield<23> up;
            Bitfield<22> psruser;
            Bitfield<21> writeback;
            Bitfield<20> loadOp;
        EndSubBitUnion(puswl)

        Bitfield<24, 20> pubwl;

        Bitfield<7, 0> imm;

        Bitfield<11, 8>  rotate;

        Bitfield<11, 0>  immed11_0;
        Bitfield<7,  0>  immed7_0;

        Bitfield<11, 8>  immedHi11_8;
        Bitfield<3,  0>  immedLo3_0;

        Bitfield<15, 0>  regList;
        
        Bitfield<23, 0>  offset;
        
        Bitfield<23, 0>  immed23_0;

        Bitfield<11, 8>  cpNum;
        Bitfield<18, 16> fn;
        Bitfield<14, 12> fd;
        Bitfield<3>      fpRegImm;
        Bitfield<3,  0>  fm;
        Bitfield<2,  0>  fpImm;
        Bitfield<24, 20> punwl;

        Bitfield<7,  0>  m5Func;
    EndBitUnion(ExtMachInst)

    // Shift types for ARM instructions
    enum ArmShiftType {
        LSL = 0,
        LSR,
        ASR,
        ROR
    };

    typedef uint64_t LargestRead;
    // Need to use 64 bits to make sure that read requests get handled properly

    typedef int RegContextParam;
    typedef int RegContextVal;

    //used in FP convert & round function
    enum ConvertType{
        SINGLE_TO_DOUBLE,
        SINGLE_TO_WORD,
        SINGLE_TO_LONG,

        DOUBLE_TO_SINGLE,
        DOUBLE_TO_WORD,
        DOUBLE_TO_LONG,

        LONG_TO_SINGLE,
        LONG_TO_DOUBLE,
        LONG_TO_WORD,
        LONG_TO_PS,

        WORD_TO_SINGLE,
        WORD_TO_DOUBLE,
        WORD_TO_LONG,
        WORD_TO_PS,

        PL_TO_SINGLE,
        PU_TO_SINGLE
    };

    //used in FP convert & round function
    enum RoundMode{
        RND_ZERO,
        RND_DOWN,
        RND_UP,
        RND_NEAREST
    };

    enum OperatingMode {
        MODE_USER = 16,
        MODE_FIQ = 17,
        MODE_IRQ = 18,
        MODE_SVC = 19,
        MODE_MON = 22,
        MODE_ABORT = 23,
        MODE_UNDEFINED = 27,
        MODE_SYSTEM = 31
    };

    struct CoreSpecific {
        // Empty for now on the ARM
    };

} // namespace ArmISA

#endif
