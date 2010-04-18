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

#ifndef __ARCH_X86_INSTS_MACROOP_HH__
#define __ARCH_X86_INSTS_MACROOP_HH__

#include "arch/x86/emulenv.hh"
#include "arch/x86/types.hh"
#include "arch/x86/insts/static_inst.hh"

namespace X86ISA
{
// Base class for combinationally generated macroops
class MacroopBase : public X86StaticInst
{
  protected:
    const char *macrocodeBlock;

    const uint32_t numMicroops;
    X86ISA::EmulEnv env;

    //Constructor.
    MacroopBase(const char *mnem, ExtMachInst _machInst,
            uint32_t _numMicroops, X86ISA::EmulEnv _env) :
                X86StaticInst(mnem, _machInst, No_OpClass),
                numMicroops(_numMicroops), env(_env)
    {
        assert(numMicroops);
        microops = new StaticInstPtr[numMicroops];
        flags[IsMacroop] = true;
    }

    ~MacroopBase()
    {
        delete [] microops;
    }

    StaticInstPtr * microops;

    StaticInstPtr fetchMicroop(MicroPC microPC)
    {
        assert(microPC < numMicroops);
        return microops[microPC];
    }

    std::string
    generateDisassembly(Addr pc, const SymbolTable *symtab) const
    {
        return mnemonic;
    }

  public:
    ExtMachInst
    getExtMachInst()
    {
        return machInst;
    }

    X86ISA::EmulEnv
    getEmulEnv()
    {
        return env;
    }
};
}

#endif //__ARCH_X86_INSTS_MACROOP_HH__
