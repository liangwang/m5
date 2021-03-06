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
 * Authors: Korey Sewell
 */

#ifndef __CPU_O3_LSQ_HH__
#define __CPU_O3_LSQ_HH__

#include <map>
#include <queue>

#include "config/full_system.hh"
#include "cpu/inst_seq.hh"
#include "cpu/o3/lsq_unit.hh"
#include "mem/port.hh"
#include "sim/sim_object.hh"

class DerivO3CPUParams;

template <class Impl>
class LSQ {
  public:
    typedef typename Impl::O3CPU O3CPU;
    typedef typename Impl::DynInstPtr DynInstPtr;
    typedef typename Impl::CPUPol::IEW IEW;
    typedef typename Impl::CPUPol::LSQUnit LSQUnit;

    /** SMT policy. */
    enum LSQPolicy {
        Dynamic,
        Partitioned,
        Threshold
    };

    /** Constructs an LSQ with the given parameters. */
    LSQ(O3CPU *cpu_ptr, IEW *iew_ptr, DerivO3CPUParams *params);

    /** Returns the name of the LSQ. */
    std::string name() const;

    /** Registers statistics of each LSQ unit. */
    void regStats();

    /** Returns dcache port.
     *  @todo: Dcache port needs to be moved up to this level for SMT
     *  to work.  For now it just returns the port from one of the
     *  threads.
     */
    Port *getDcachePort() { return &dcachePort; }

    /** Sets the pointer to the list of active threads. */
    void setActiveThreads(std::list<ThreadID> *at_ptr);
    /** Switches out the LSQ. */
    void switchOut();
    /** Takes over execution from another CPU's thread. */
    void takeOverFrom();

    /** Number of entries needed for the given amount of threads.*/
    int entryAmount(ThreadID num_threads);
    void removeEntries(ThreadID tid);
    /** Reset the max entries for each thread. */
    void resetEntries();
    /** Resize the max entries for a thread. */
    void resizeEntries(unsigned size, ThreadID tid);

    /** Ticks the LSQ. */
    void tick();
    /** Ticks a specific LSQ Unit. */
    void tick(ThreadID tid)
    { thread[tid].tick(); }

    /** Inserts a load into the LSQ. */
    void insertLoad(DynInstPtr &load_inst);
    /** Inserts a store into the LSQ. */
    void insertStore(DynInstPtr &store_inst);

    /** Executes a load. */
    Fault executeLoad(DynInstPtr &inst);

    /** Executes a store. */
    Fault executeStore(DynInstPtr &inst);

    /**
     * Commits loads up until the given sequence number for a specific thread.
     */
    void commitLoads(InstSeqNum &youngest_inst, ThreadID tid)
    { thread[tid].commitLoads(youngest_inst); }

    /**
     * Commits stores up until the given sequence number for a specific thread.
     */
    void commitStores(InstSeqNum &youngest_inst, ThreadID tid)
    { thread[tid].commitStores(youngest_inst); }

    /**
     * Attempts to write back stores until all cache ports are used or the
     * interface becomes blocked.
     */
    void writebackStores();
    /** Same as above, but only for one thread. */
    void writebackStores(ThreadID tid);

    /**
     * Squash instructions from a thread until the specified sequence number.
     */
    void squash(const InstSeqNum &squashed_num, ThreadID tid)
    { thread[tid].squash(squashed_num); }

    /** Returns whether or not there was a memory ordering violation. */
    bool violation();
    /**
     * Returns whether or not there was a memory ordering violation for a
     * specific thread.
     */
    bool violation(ThreadID tid)
    { return thread[tid].violation(); }

    /** Returns if a load is blocked due to the memory system for a specific
     *  thread.
     */
    bool loadBlocked(ThreadID tid)
    { return thread[tid].loadBlocked(); }

    bool isLoadBlockedHandled(ThreadID tid)
    { return thread[tid].isLoadBlockedHandled(); }

    void setLoadBlockedHandled(ThreadID tid)
    { thread[tid].setLoadBlockedHandled(); }

    /** Gets the instruction that caused the memory ordering violation. */
    DynInstPtr getMemDepViolator(ThreadID tid)
    { return thread[tid].getMemDepViolator(); }

    /** Returns the head index of the load queue for a specific thread. */
    int getLoadHead(ThreadID tid)
    { return thread[tid].getLoadHead(); }

    /** Returns the sequence number of the head of the load queue. */
    InstSeqNum getLoadHeadSeqNum(ThreadID tid)
    {
        return thread[tid].getLoadHeadSeqNum();
    }

    /** Returns the head index of the store queue. */
    int getStoreHead(ThreadID tid)
    { return thread[tid].getStoreHead(); }

    /** Returns the sequence number of the head of the store queue. */
    InstSeqNum getStoreHeadSeqNum(ThreadID tid)
    {
        return thread[tid].getStoreHeadSeqNum();
    }

    /** Returns the number of instructions in all of the queues. */
    int getCount();
    /** Returns the number of instructions in the queues of one thread. */
    int getCount(ThreadID tid)
    { return thread[tid].getCount(); }

    /** Returns the total number of loads in the load queue. */
    int numLoads();
    /** Returns the total number of loads for a single thread. */
    int numLoads(ThreadID tid)
    { return thread[tid].numLoads(); }

    /** Returns the total number of stores in the store queue. */
    int numStores();
    /** Returns the total number of stores for a single thread. */
    int numStores(ThreadID tid)
    { return thread[tid].numStores(); }

    /** Returns the total number of loads that are ready. */
    int numLoadsReady();
    /** Returns the number of loads that are ready for a single thread. */
    int numLoadsReady(ThreadID tid)
    { return thread[tid].numLoadsReady(); }

    /** Returns the number of free entries. */
    unsigned numFreeEntries();
    /** Returns the number of free entries for a specific thread. */
    unsigned numFreeEntries(ThreadID tid);

    /** Returns if the LSQ is full (either LQ or SQ is full). */
    bool isFull();
    /**
     * Returns if the LSQ is full for a specific thread (either LQ or SQ is
     * full).
     */
    bool isFull(ThreadID tid);

    /** Returns if any of the LQs are full. */
    bool lqFull();
    /** Returns if the LQ of a given thread is full. */
    bool lqFull(ThreadID tid);

    /** Returns if any of the SQs are full. */
    bool sqFull();
    /** Returns if the SQ of a given thread is full. */
    bool sqFull(ThreadID tid);

    /**
     * Returns if the LSQ is stalled due to a memory operation that must be
     * replayed.
     */
    bool isStalled();
    /**
     * Returns if the LSQ of a specific thread is stalled due to a memory
     * operation that must be replayed.
     */
    bool isStalled(ThreadID tid);

    /** Returns whether or not there are any stores to write back to memory. */
    bool hasStoresToWB();

    /** Returns whether or not a specific thread has any stores to write back
     * to memory.
     */
    bool hasStoresToWB(ThreadID tid)
    { return thread[tid].hasStoresToWB(); }

    /** Returns the number of stores a specific thread has to write back. */
    int numStoresToWB(ThreadID tid)
    { return thread[tid].numStoresToWB(); }

    /** Returns if the LSQ will write back to memory this cycle. */
    bool willWB();
    /** Returns if the LSQ of a specific thread will write back to memory this
     * cycle.
     */
    bool willWB(ThreadID tid)
    { return thread[tid].willWB(); }

    /** Returns if the cache is currently blocked. */
    bool cacheBlocked()
    { return retryTid != InvalidThreadID; }

    /** Sets the retry thread id, indicating that one of the LSQUnits
     * tried to access the cache but the cache was blocked. */
    void setRetryTid(ThreadID tid)
    { retryTid = tid; }

    /** Debugging function to print out all instructions. */
    void dumpInsts();
    /** Debugging function to print out instructions from a specific thread. */
    void dumpInsts(ThreadID tid)
    { thread[tid].dumpInsts(); }

    /** Executes a read operation, using the load specified at the load
     * index.
     */
    template <class T>
    Fault read(RequestPtr req, RequestPtr sreqLow, RequestPtr sreqHigh,
               T &data, int load_idx);

    /** Executes a store operation, using the store specified at the store
     * index.
     */
    template <class T>
    Fault write(RequestPtr req, RequestPtr sreqLow, RequestPtr sreqHigh,
                T &data, int store_idx);

    /** The CPU pointer. */
    O3CPU *cpu;

    /** The IEW stage pointer. */
    IEW *iewStage;

    /** DcachePort class for this LSQ.  Handles doing the
     * communication with the cache/memory.
     */
    class DcachePort : public Port
    {
      protected:
        /** Pointer to LSQ. */
        LSQ *lsq;

      public:
        /** Default constructor. */
        DcachePort(LSQ *_lsq)
            : Port(_lsq->name() + "-dport", _lsq->cpu), lsq(_lsq)
        { }

        bool snoopRangeSent;

        virtual void setPeer(Port *port);

      protected:
        /** Atomic version of receive.  Panics. */
        virtual Tick recvAtomic(PacketPtr pkt);

        /** Functional version of receive.  Panics. */
        virtual void recvFunctional(PacketPtr pkt);

        /** Receives status change.  Other than range changing, panics. */
        virtual void recvStatusChange(Status status);

        /** Returns the address ranges of this device. */
        virtual void getDeviceAddressRanges(AddrRangeList &resp,
                                            bool &snoop)
        { resp.clear(); snoop = true; }

        /** Timing version of receive.  Handles writing back and
         * completing the load or store that has returned from
         * memory. */
        virtual bool recvTiming(PacketPtr pkt);

        /** Handles doing a retry of the previous send. */
        virtual void recvRetry();
    };

    /** D-cache port. */
    DcachePort dcachePort;

#if FULL_SYSTEM
    /** Tell the CPU to update the Phys and Virt ports. */
    void updateMemPorts() { cpu->updateMemPorts(); }
#endif

  protected:
    /** The LSQ policy for SMT mode. */
    LSQPolicy lsqPolicy;

    /** The LSQ units for individual threads. */
    LSQUnit thread[Impl::MaxThreads];

    /** List of Active Threads in System. */
    std::list<ThreadID> *activeThreads;

    /** Total Size of LQ Entries. */
    unsigned LQEntries;
    /** Total Size of SQ Entries. */
    unsigned SQEntries;

    /** Max LQ Size - Used to Enforce Sharing Policies. */
    unsigned maxLQEntries;

    /** Max SQ Size - Used to Enforce Sharing Policies. */
    unsigned maxSQEntries;

    /** Number of Threads. */
    ThreadID numThreads;

    /** The thread id of the LSQ Unit that is currently waiting for a
     * retry. */
    ThreadID retryTid;
};

template <class Impl>
template <class T>
Fault
LSQ<Impl>::read(RequestPtr req, RequestPtr sreqLow, RequestPtr sreqHigh,
                T &data, int load_idx)
{
    ThreadID tid = req->threadId();

    return thread[tid].read(req, sreqLow, sreqHigh, data, load_idx);
}

template <class Impl>
template <class T>
Fault
LSQ<Impl>::write(RequestPtr req, RequestPtr sreqLow, RequestPtr sreqHigh,
                 T &data, int store_idx)
{
    ThreadID tid = req->threadId();

    return thread[tid].write(req, sreqLow, sreqHigh, data, store_idx);
}

#endif // __CPU_O3_LSQ_HH__
