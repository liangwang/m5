/*
 * Copyright (c) 2007 The Hewlett-Packard Development Company
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
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
 * Authors: Gabe Black
 */

#include "arch/x86/insts/microregop.hh"
#include "arch/x86/miscregs.hh"
#include "base/condcodes.hh"
#include <string>

namespace X86ISA
{
    uint64_t RegOpBase::genFlags(uint64_t oldFlags, uint64_t flagMask,
            uint64_t _dest, uint64_t _src1, uint64_t _src2,
            bool subtract) const
    {
        DPRINTF(X86, "flagMask = %#x\n", flagMask);
        if (_destRegIdx[0] & IntFoldBit) {
            _dest >>= 8;
        }
        uint64_t flags = oldFlags & ~flagMask;
        if(flagMask & (ECFBit | CFBit))
        {
            if(findCarry(dataSize*8, _dest, _src1, _src2))
                flags |= (flagMask & (ECFBit | CFBit));
            if(subtract)
                flags ^= (flagMask & (ECFBit | CFBit));
        }
        if(flagMask & PFBit && !findParity(8, _dest))
            flags |= PFBit;
        if(flagMask & AFBit)
        {
            if(findCarry(4, _dest, _src1, _src2))
                flags |= AFBit;
            if(subtract)
                flags ^= AFBit;
        }
        if(flagMask & (EZFBit | ZFBit) && findZero(dataSize*8, _dest))
            flags |= (flagMask & (EZFBit | ZFBit));
        if(flagMask & SFBit && findNegative(dataSize*8, _dest))
            flags |= SFBit;
        if(flagMask & OFBit && findOverflow(dataSize*8, _dest, _src1, _src2))
            flags |= OFBit;
        return flags;
    }

    std::string RegOp::generateDisassembly(Addr pc,
            const SymbolTable *symtab) const
    {
        std::stringstream response;

        printMnemonic(response, instMnem, mnemonic);
        printDestReg(response, 0, dataSize);
        response << ", ";
        printSrcReg(response, 0, dataSize);
        response << ", ";
        printSrcReg(response, 1, dataSize);
        return response.str();
    }

    std::string RegOpImm::generateDisassembly(Addr pc,
            const SymbolTable *symtab) const
    {
        std::stringstream response;

        printMnemonic(response, instMnem, mnemonic);
        printDestReg(response, 0, dataSize);
        response << ", ";
        printSrcReg(response, 0, dataSize);
        ccprintf(response, ", %#x", imm8);
        return response.str();
    }
}
