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

#ifndef __ARCH_X86_PROCESS_HH__
#define __ARCH_X86_PROCESS_HH__

#include <string>
#include <vector>
#include "sim/process.hh"

class SyscallDesc;

namespace X86ISA
{

    class X86LiveProcess : public LiveProcess
    {
      protected:
        Addr _gdtStart;
        Addr _gdtSize;

        SyscallDesc *syscallDescs;
        const int numSyscallDescs;

        X86LiveProcess(LiveProcessParams * params, ObjectFile *objFile,
                SyscallDesc *_syscallDescs, int _numSyscallDescs);

        template<class IntType>
        void argsInit(int pageSize,
                std::vector<AuxVector<IntType> > extraAuxvs);

      public:
        Addr gdtStart()
        { return _gdtStart; }
        
        Addr gdtSize()
        { return _gdtSize; }

        SyscallDesc* getDesc(int callnum);

        void setSyscallReturn(ThreadContext *tc, SyscallReturn return_value);
    };

    class X86_64LiveProcess : public X86LiveProcess
    {
      protected:
        X86_64LiveProcess(LiveProcessParams *params, ObjectFile *objFile,
                SyscallDesc *_syscallDescs, int _numSyscallDescs);

        class VSyscallPage
        {
          public:
            Addr base;
            Addr size;
            Addr vtimeOffset;
            Addr vgettimeofdayOffset;
        };
        VSyscallPage vsyscallPage;

      public:
        void argsInit(int intSize, int pageSize);
        void startup();

        X86ISA::IntReg getSyscallArg(ThreadContext *tc, int &i);
        void setSyscallArg(ThreadContext *tc, int i, X86ISA::IntReg val);
    };

    class I386LiveProcess : public X86LiveProcess
    {
      protected:
        I386LiveProcess(LiveProcessParams *params, ObjectFile *objFile,
                SyscallDesc *_syscallDescs, int _numSyscallDescs);

        class VSyscallPage
        {
          public:
            Addr base;
            Addr size;
            Addr vsyscallOffset;
            Addr vsysexitOffset;
        };
        VSyscallPage vsyscallPage;

      public:
        void argsInit(int intSize, int pageSize);
        void startup();

        void syscall(int64_t callnum, ThreadContext *tc);
        X86ISA::IntReg getSyscallArg(ThreadContext *tc, int &i);
        X86ISA::IntReg getSyscallArg(ThreadContext *tc, int &i, int width);
        void setSyscallArg(ThreadContext *tc, int i, X86ISA::IntReg val);
    };
}

#endif // __ARCH_X86_PROCESS_HH__
