/*
 * Copyright (c) 2006 The Regents of The University of Michigan
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
 */

#ifndef __SIM_TLB_HH__
#define __SIM_TLB_HH__

#include "base/misc.hh"
#include "mem/request.hh"
#include "sim/faults.hh"
#include "sim/sim_object.hh"

class ThreadContext;
class Packet;

class BaseTLB : public SimObject
{
  protected:
    BaseTLB(const Params *p)
        : SimObject(p)
    {}

  public:
    enum Mode { Read, Write, Execute };

  public:
    virtual void demapPage(Addr vaddr, uint64_t asn) = 0;

    class Translation
    {
      public:
        virtual ~Translation()
        {}

        /*
         * The memory for this object may be dynamically allocated, and it may
         * be responsible for cleaning itself up which will happen in this
         * function. Once it's called, the object is no longer valid.
         */
        virtual void finish(Fault fault, RequestPtr req, ThreadContext *tc,
                            Mode mode) = 0;
    };
};

class GenericTLB : public BaseTLB
{
  protected:
    GenericTLB(const Params *p)
        : BaseTLB(p)
    {}

  public:
    void demapPage(Addr vaddr, uint64_t asn);

    Fault translateAtomic(RequestPtr req, ThreadContext *tc, Mode mode);
    void translateTiming(RequestPtr req, ThreadContext *tc,
                         Translation *translation, Mode mode);
};

#endif // __ARCH_SPARC_TLB_HH__
