/*
 * Copyright (c) 2004-2005 The Regents of The University of Michigan
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

#ifndef __CPU_O3LITE_CPU_POLICY_HH__
#define __CPU_O3LITE_CPU_POLICY_HH__

#include "cpu/o3lite/bpred_unit.hh"
#include "cpu/o3lite/free_list.hh"
#include "cpu/o3lite/inst_queue.hh"
#include "cpu/o3lite/lsq.hh"
#include "cpu/o3lite/lsq_unit.hh"
#include "cpu/o3lite/mem_dep_unit.hh"
#include "cpu/o3lite/regfile.hh"
#include "cpu/o3lite/rename_map.hh"
#include "cpu/o3lite/rob.hh"
#include "cpu/o3lite/store_set.hh"

#include "cpu/o3lite/commit.hh"
#include "cpu/o3lite/decode.hh"
#include "cpu/o3lite/fetch.hh"
#include "cpu/o3lite/iew.hh"
#include "cpu/o3lite/rename.hh"

#include "cpu/o3lite/comm.hh"

/**
 * Struct that defines the key classes to be used by the CPU.  All
 * classes use the typedefs defined here to determine what are the
 * classes of the other stages and communication buffers.  In order to
 * change a structure such as the IQ, simply change the typedef here
 * to use the desired class instead, and recompile.  In order to
 * create a different CPU to be used simultaneously with this one, see
 * the alpha_impl.hh file for instructions.
 */
template<class Impl>
struct O3liteCPUPolicy
{
    /** Typedef for the branch prediction unit (which includes the BP,
     * RAS, and BTB).
     */
    typedef ::O3liteBPredUnit<Impl> BPredUnit;
    /** Typedef for the register file.  Most classes assume a unified
     * physical register file.
     */
    typedef O3litePhysRegFile<Impl> RegFile;
    /** Typedef for the freelist of registers. */
    typedef O3liteFreeList FreeList;
    /** Typedef for the rename map. */
    typedef O3liteRenameMap RenameMap;
    /** Typedef for the ROB. */
    typedef ::O3liteROB<Impl> ROB;
    /** Typedef for the instruction queue/scheduler. */
    typedef O3liteInstructionQueue<Impl> IQ;
    /** Typedef for the memory dependence unit. */
    typedef ::O3liteMemDepUnit<O3liteStoreSet, Impl> MemDepUnit;
    /** Typedef for the LSQ. */
    typedef ::O3liteLSQ<Impl> LSQ;
    /** Typedef for the thread-specific LSQ units. */
    typedef ::O3liteLSQUnit<Impl> LSQUnit;

    typedef ::O3liteScoreboard Scoreboard;

    /** Typedef for fetch. */
    typedef O3liteFetch<Impl> Fetch;
    /** Typedef for decode. */
    typedef O3liteDecode<Impl> Decode;
    /** Typedef for rename. */
    typedef O3liteRename<Impl> Rename;
    /** Typedef for Issue/Execute/Writeback. */
    typedef O3liteIEW<Impl> IEW;
    /** Typedef for commit. */
    typedef O3liteCommit<Impl> Commit;

    /** The struct for communication between fetch and decode. */
    typedef O3liteFetchDefaultDecode<Impl> FetchStruct;

    /** The struct for communication between decode and rename. */
    typedef O3liteDecodeDefaultRename<Impl> DecodeStruct;

    /** The struct for communication between rename and IEW. */
    typedef O3liteRenameDefaultIEW<Impl> RenameStruct;

    /** The struct for communication between IEW and commit. */
    typedef O3liteIEWDefaultCommit<Impl> IEWStruct;

    /** The struct for communication within the IEW stage. */
    typedef ::O3liteIssueStruct<Impl> IssueStruct;

    /** The struct for all backwards communication. */
    typedef O3liteTimeBufStruct<Impl> TimeStruct;

};

#endif //__CPU_O3LITE_CPU_POLICY_HH__
