/*
 * Copyright (c) 2004-2006 The Regents of The University of Michigan
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
 * Authors: Kevin Lim
 */

#include <string>

#include "config/full_system.hh"
#include "config/use_checker.hh"
#include "cpu/o3lite/cpu.hh"
#include "cpu/o3lite/impl.hh"
#include "params/DerivO3liteCPU.hh"

class DerivO3liteCPU : public FullO3liteCPU<O3CPUImpl>
{
  public:
    DerivO3liteCPU(DerivO3liteCPUParams *p)
        : FullO3liteCPU<O3CPUImpl>(p)
    { }
};

DerivO3liteCPU *
DerivO3liteCPUParams::create()
{
#if FULL_SYSTEM
    // Full-system only supports a single thread for the moment.
    ThreadID actual_num_threads = 1;
#else
    if (workload.size() > numThreads) {
        fatal("Workload Size (%i) > Max Supported Threads (%i) on This CPU",
              workload.size(), numThreads);
    } else if (workload.size() == 0) {
        fatal("Must specify at least one workload!");
    }

    // In non-full-system mode, we infer the number of threads from
    // the workload if it's not explicitly specified.
    ThreadID actual_num_threads =
        (numThreads >= workload.size()) ? numThreads : workload.size();
#endif

    numThreads = actual_num_threads;

    // Default smtFetchPolicy to "RoundRobin", if necessary.
    std::string round_robin_policy = "RoundRobin";
    std::string single_thread = "SingleThread";

    if (actual_num_threads > 1 && single_thread.compare(smtFetchPolicy) == 0)
        smtFetchPolicy = round_robin_policy;
    else
        smtFetchPolicy = smtFetchPolicy;

    instShiftAmt = 2;

    return new DerivO3liteCPU(this);
}
