/*
 * Copyright (c) 2006 The Regents of The University of Michigan
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
 * Authors: Gabe Black
 *          Stephen Hines
 */

#ifndef __ARCH_ARM_PREDECODER_HH__
#define __ARCH_ARM_PREDECODER_HH__

#include "arch/arm/types.hh"
#include "base/misc.hh"
#include "base/types.hh"

class ThreadContext;

namespace ArmISA
{
    class Predecoder
    {
      protected:
        ThreadContext * tc;
        //The extended machine instruction being generated
        ExtMachInst emi;

      public:
        Predecoder(ThreadContext * _tc) : tc(_tc)
        {}

        ThreadContext * getTC()
        {
            return tc;
        }

        void setTC(ThreadContext * _tc)
        {
            tc = _tc;
        }

        void process()
        {}

        void reset()
        {}

        //Use this to give data to the predecoder. This should be used
        //when there is control flow.
        void moreBytes(Addr pc, Addr fetchPC, MachInst inst)
        {
            emi = inst;
            emi.sevenAndFour = bits(inst, 7) && bits(inst, 4);
            emi.isMisc = (bits(inst, 24, 23) == 0x2 && bits(inst, 20) == 0);
        }

        //Use this to give data to the predecoder. This should be used
        //when instructions are executed in order.
        void moreBytes(MachInst machInst)
        {
            moreBytes(0, 0, machInst);
        }

        bool needMoreBytes()
        {
            return true;
        }

        bool extMachInstReady()
        {
            return true;
        }

        //This returns a constant reference to the ExtMachInst to avoid a copy
        const ExtMachInst & getExtMachInst()
        {
            return emi;
        }
    };
};

#endif // __ARCH_ARM_PREDECODER_HH__
