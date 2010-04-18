/*
 * Copyright (c) 2007 The Hewlett-Packard Development Company
 * All rights reserved.
 *
 * Redistribution and use of this software in source and binary forms,
 * with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * The software must be used only for Non-Commercial Use which means any
 * use which is NOT directed to receiving any direct monetary
 * compensation for, or commercial advantage from such use.  Illustrative
 * examples of non-commercial use are academic research, personal study,
 * teaching, education and corporate research & development.
 * Illustrative examples of commercial use are distributing products for
 * commercial advantage and providing services using the software for
 * commercial advantage.
 *
 * If you wish to use this software or functionality therein that may be
 * covered by patents for commercial use, please contact:
 *     Director of Intellectual Property Licensing
 *     Office of Strategy and Technology
 *     Hewlett-Packard Company
 *     1501 Page Mill Road
 *     Palo Alto, California  94304
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.  Redistributions
 * in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.  Neither the name of
 * the COPYRIGHT HOLDER(s), HEWLETT-PACKARD COMPANY, nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.  No right of
 * sublicense is granted herewith.  Derivatives of the software and
 * output created using the software may be prepared, but only for
 * Non-Commercial Uses.  Derivatives of the software may be shared with
 * others provided: (i) the others agree to abide by the list of
 * conditions herein which includes the Non-Commercial Use restrictions;
 * and (ii) such Derivatives of the software include the above copyright
 * notice to acknowledge the contribution from this software where
 * applicable, this list of conditions and the disclaimer below.
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
 * Authors: Gabe Black
 */

#ifndef __ARCH_X86_TYPES_HH__
#define __ARCH_X86_TYPES_HH__

#include <iostream>

#include "base/bitunion.hh"
#include "base/cprintf.hh"
#include "base/types.hh"

namespace X86ISA
{
    //This really determines how many bytes are passed to the predecoder.
    typedef uint64_t MachInst;

    enum Prefixes {
        NoOverride,
        ESOverride,
        CSOverride,
        SSOverride,
        DSOverride,
        FSOverride,
        GSOverride,
        RexPrefix,
        OperandSizeOverride,
        AddressSizeOverride,
        Lock,
        Rep,
        Repne
    };

    BitUnion8(LegacyPrefixVector)
        Bitfield<7, 4> decodeVal;
        Bitfield<7> repne;
        Bitfield<6> rep;
        Bitfield<5> lock;
        Bitfield<4> op;
        Bitfield<3> addr;
        //There can be only one segment override, so they share the
        //first 3 bits in the legacyPrefixes bitfield.
        Bitfield<2,0> seg;
    EndBitUnion(LegacyPrefixVector)

    BitUnion8(ModRM)
        Bitfield<7,6> mod;
        Bitfield<5,3> reg;
        Bitfield<2,0> rm;
    EndBitUnion(ModRM)

    BitUnion8(Sib)
        Bitfield<7,6> scale;
        Bitfield<5,3> index;
        Bitfield<2,0> base;
    EndBitUnion(Sib)

    BitUnion8(Rex)
        //This bit doesn't mean anything according to the ISA, but in
        //this implementation, it being set means an REX prefix was present.
        Bitfield<6> present;
        Bitfield<3> w;
        Bitfield<2> r;
        Bitfield<1> x;
        Bitfield<0> b;
    EndBitUnion(Rex)

    BitUnion8(Opcode)
        Bitfield<7,3> top5;
        Bitfield<2,0> bottom3;
    EndBitUnion(Opcode)

    BitUnion8(OperatingMode)
        Bitfield<3> mode;
        Bitfield<2,0> submode;
    EndBitUnion(OperatingMode)

    enum X86Mode {
        LongMode,
        LegacyMode
    };

    enum X86SubMode {
        SixtyFourBitMode,
        CompatabilityMode,
        ProtectedMode,
        Virtual8086Mode,
        RealMode
    };

    //The intermediate structure the x86 predecoder returns.
    struct ExtMachInst
    {
        //Prefixes
        LegacyPrefixVector legacy;
        Rex rex;
        //This holds all of the bytes of the opcode
        struct
        {
            //The number of bytes in this opcode. Right now, we ignore that
            //this can be 3 in some cases
            uint8_t num;
            //The first byte detected in a 2+ byte opcode. Should be 0xF0.
            uint8_t prefixA;
            //The second byte detected in a 3+ byte opcode. Could be 0x38-0x3F
            //for some SSE instructions. 3dNow! instructions are handled as
            //two byte opcodes and then split out further by the immediate
            //byte.
            uint8_t prefixB;
            //The main opcode byte. The highest addressed byte in the opcode.
            Opcode op;
        } opcode;
        //Modifier bytes
        ModRM modRM;
        Sib sib;
        //Immediate fields
        uint64_t immediate;
        uint64_t displacement;

        //The effective operand size.
        uint8_t opSize;
        //The effective address size.
        uint8_t addrSize;
        //The effective stack size.
        uint8_t stackSize;
        //The size of the displacement
        uint8_t dispSize;

        //Mode information
        OperatingMode mode;
    };

    inline static std::ostream &
        operator << (std::ostream & os, const ExtMachInst & emi)
    {
        ccprintf(os, "\n{\n\tleg = %#x,\n\trex = %#x,\n\t"
                     "op = {\n\t\tnum = %d,\n\t\top = %#x,\n\t\t"
                           "prefixA = %#x,\n\t\tprefixB = %#x\n\t},\n\t"
                     "modRM = %#x,\n\tsib = %#x,\n\t"
                     "immediate = %#x,\n\tdisplacement = %#x\n\t"
                     "dispSize = %d}\n",
                     (uint8_t)emi.legacy, (uint8_t)emi.rex,
                     emi.opcode.num, (uint8_t)emi.opcode.op,
                     emi.opcode.prefixA, emi.opcode.prefixB,
                     (uint8_t)emi.modRM, (uint8_t)emi.sib,
                     emi.immediate, emi.displacement, emi.dispSize);
        return os;
    }

    inline static bool
        operator == (const ExtMachInst &emi1, const ExtMachInst &emi2)
    {
        if(emi1.legacy != emi2.legacy)
            return false;
        if(emi1.rex != emi2.rex)
            return false;
        if(emi1.opcode.num != emi2.opcode.num)
            return false;
        if(emi1.opcode.op != emi2.opcode.op)
            return false;
        if(emi1.opcode.prefixA != emi2.opcode.prefixA)
            return false;
        if(emi1.opcode.prefixB != emi2.opcode.prefixB)
            return false;
        if(emi1.modRM != emi2.modRM)
            return false;
        if(emi1.sib != emi2.sib)
            return false;
        if(emi1.immediate != emi2.immediate)
            return false;
        if(emi1.displacement != emi2.displacement)
            return false;
        if(emi1.mode != emi2.mode)
            return false;
        if(emi1.opSize != emi2.opSize)
            return false;
        if(emi1.addrSize != emi2.addrSize)
            return false;
        if(emi1.stackSize != emi2.stackSize)
            return false;
        if(emi1.dispSize != emi2.dispSize)
            return false;
        return true;
    }

    struct CoreSpecific {
        int core_type;
    };
};

#endif // __ARCH_X86_TYPES_HH__
