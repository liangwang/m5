/*
 * Copyright (c) 2005-2006 The Regents of The University of Michigan
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

#include <algorithm>
#include <list>
#include <string>

#include "cpu/o3lite/lsq.hh"
#include "params/DerivO3liteCPU.hh"

using namespace std;

template<class Impl>
void
O3liteLSQ<Impl>::DcachePort::setPeer(Port *port)
{
    Port::setPeer(port);

#if FULL_SYSTEM
    // Update the ThreadContext's memory ports (Functional/Virtual
    // Ports)
    lsq->updateMemPorts();
#endif
}

template <class Impl>
Tick
O3liteLSQ<Impl>::DcachePort::recvAtomic(PacketPtr pkt)
{
    panic("O3CPU model does not work with atomic mode!");
    return curTick;
}

template <class Impl>
void
O3liteLSQ<Impl>::DcachePort::recvFunctional(PacketPtr pkt)
{
    DPRINTF(LSQ, "LSQ doesn't update things on a recvFunctional.");
}

template <class Impl>
void
O3liteLSQ<Impl>::DcachePort::recvStatusChange(Status status)
{
    if (status == RangeChange) {
        if (!snoopRangeSent) {
            snoopRangeSent = true;
            sendStatusChange(Port::RangeChange);
        }
        return;
    }
    panic("O3CPU doesn't expect recvStatusChange callback!");
}

template <class Impl>
bool
O3liteLSQ<Impl>::DcachePort::recvTiming(PacketPtr pkt)
{
    if (pkt->isError())
        DPRINTF(LSQ, "Got error packet back for address: %#X\n", pkt->getAddr());
    if (pkt->isResponse()) {
        lsq->thread[pkt->req->threadId()].completeDataAccess(pkt);
    }
    else {
        // must be a snoop

        // @TODO someday may need to process invalidations in LSQ here
        // to provide stronger consistency model
    }
    return true;
}

template <class Impl>
void
O3liteLSQ<Impl>::DcachePort::recvRetry()
{
    if (lsq->retryTid == -1)
    {
        //Squashed, so drop it
        return;
    }
    int curr_retry_tid = lsq->retryTid;
    // Speculatively clear the retry Tid.  This will get set again if
    // the LSQUnit was unable to complete its access.
    lsq->retryTid = -1;
    lsq->thread[curr_retry_tid].recvRetry();
}

template <class Impl>
O3liteLSQ<Impl>::O3liteLSQ(O3CPU *cpu_ptr, IEW *iew_ptr, DerivO3liteCPUParams *params)
    : cpu(cpu_ptr), iewStage(iew_ptr), dcachePort(this),
      LQEntries(params->LQEntries),
      SQEntries(params->SQEntries),
      MATEntries(params->MATEntries),
      SBEntries(params->SBEntries),
      numThreads(params->numThreads),
      retryTid(-1)
{
    dcachePort.snoopRangeSent = false;

    //**********************************************/
    //************ Handle SMT Parameters ***********/
    //**********************************************/
    std::string policy = params->smtLSQPolicy;

    //Convert string to lowercase
    std::transform(policy.begin(), policy.end(), policy.begin(),
                   (int(*)(int)) tolower);

    //Figure out fetch policy
    if (policy == "dynamic") {
        lsqPolicy = Dynamic;

        maxLQEntries = LQEntries;
        maxSQEntries = SQEntries;
        maxMATEntries = MATEntries;
        maxSBEntries = SBEntries;

        DPRINTF(LSQ, "LSQ sharing policy set to Dynamic\n");
    } else if (policy == "partitioned") {
        lsqPolicy = Partitioned;

        //@todo:make work if part_amt doesnt divide evenly.
        maxLQEntries = LQEntries / numThreads;
        maxSQEntries = SQEntries / numThreads;
        maxMATEntries= MATEntries / numThreads;
        maxSBEntries = SBEntries / numThreads;

        DPRINTF(Fetch, "LSQ sharing policy set to Partitioned: "
                "%i entries per LQ | %i entries per SQ\n",
                maxLQEntries,maxSQEntries);
    } else if (policy == "threshold") {
        lsqPolicy = Threshold;

        assert(params->smtLSQThreshold > LQEntries);
        assert(params->smtLSQThreshold > SQEntries);
        assert(params->smtLSQThreshold > MATEntries);

        //Divide up by threshold amount
        //@todo: Should threads check the max and the total
        //amount of the LSQ
        maxLQEntries  = params->smtLSQThreshold;
        maxSQEntries  = params->smtLSQThreshold;
        maxMATEntries = params->smtMATThreshold;

        DPRINTF(LSQ, "LSQ sharing policy set to Threshold: "
                "%i entries per LQ | %i entries per SQ",
                maxLQEntries,maxSQEntries);
    } else {
        assert(0 && "Invalid LSQ Sharing Policy.Options Are:{Dynamic,"
                    "Partitioned, Threshold}");
    }

    //Initialize LSQs
    for (ThreadID tid = 0; tid < numThreads; tid++) {
        thread[tid].init(cpu, iew_ptr, params, this,
                         maxLQEntries, maxSQEntries,
                         maxMATEntries, maxSBEntries, tid);
        thread[tid].setDcachePort(&dcachePort);
    }
}


template<class Impl>
std::string
O3liteLSQ<Impl>::name() const
{
    return iewStage->name() + ".lsq";
}

template<class Impl>
void
O3liteLSQ<Impl>::regStats()
{
    //Initialize LSQs
    for (ThreadID tid = 0; tid < numThreads; tid++) {
        thread[tid].regStats();
    }
}

template<class Impl>
void
O3liteLSQ<Impl>::setActiveThreads(list<ThreadID> *at_ptr)
{
    activeThreads = at_ptr;
    assert(activeThreads != 0);
}

template <class Impl>
void
O3liteLSQ<Impl>::switchOut()
{
    for (ThreadID tid = 0; tid < numThreads; tid++) {
        thread[tid].switchOut();
    }
}

template <class Impl>
void
O3liteLSQ<Impl>::takeOverFrom()
{
    for (ThreadID tid = 0; tid < numThreads; tid++) {
        thread[tid].takeOverFrom();
    }
}

template <class Impl>
int
O3liteLSQ<Impl>::entryAmount(ThreadID num_threads)
{
    if (lsqPolicy == Partitioned) {
        return LQEntries / num_threads;
    } else {
        return 0;
    }
}

template <class Impl>
void
O3liteLSQ<Impl>::resetEntries()
{
    if (lsqPolicy != Dynamic || numThreads > 1) {
        int active_threads = activeThreads->size();

        int maxEntries;

        if (lsqPolicy == Partitioned) {
            maxEntries = LQEntries / active_threads;
        } else if (lsqPolicy == Threshold && active_threads == 1) {
            maxEntries = LQEntries;
        } else {
            maxEntries = LQEntries;
        }

        list<ThreadID>::iterator threads  = activeThreads->begin();
        list<ThreadID>::iterator end = activeThreads->end();

        while (threads != end) {
            ThreadID tid = *threads++;

            resizeEntries(maxEntries, tid);
        }
    }
}

template<class Impl>
void
O3liteLSQ<Impl>::removeEntries(ThreadID tid)
{
    thread[tid].clearLQ();
    thread[tid].clearSQ();
    thread[tid].clearMAT();
}

template<class Impl>
void
O3liteLSQ<Impl>::resizeEntries(unsigned size, ThreadID tid)
{
    thread[tid].resizeLQ(size);
    thread[tid].resizeSQ(size);
    thread[tid].resizeMAT(size);
}

template<class Impl>
void
O3liteLSQ<Impl>::tick()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        thread[tid].tick();
    }
}

template<class Impl>
void
O3liteLSQ<Impl>::insertLoad(DynInstPtr &load_inst)
{
    ThreadID tid = load_inst->threadNumber;

    thread[tid].insertLoad(load_inst);
}

template<class Impl>
void
O3liteLSQ<Impl>::insertStore(DynInstPtr &store_inst)
{
    ThreadID tid = store_inst->threadNumber;

    thread[tid].insertStore(store_inst);
}

template<class Impl>
Fault
O3liteLSQ<Impl>::executeLoad(DynInstPtr &inst)
{
    ThreadID tid = inst->threadNumber;

    return thread[tid].executeLoad(inst);
}

template<class Impl>
Fault
O3liteLSQ<Impl>::executeStore(DynInstPtr &inst)
{
    ThreadID tid = inst->threadNumber;

    return thread[tid].executeStore(inst);
}

template<class Impl>
void
O3liteLSQ<Impl>::writebackStores()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (numStoresToWB(tid) > 0) {
            DPRINTF(Writeback,"[tid:%i] Writing back stores. %i stores "
                "available for Writeback.\n", tid, numStoresToWB(tid));
        }

        thread[tid].writebackStores();
    }
}

template<class Impl>
bool
O3liteLSQ<Impl>::violation()
{
    /* Answers: Does Anybody Have a Violation?*/
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (thread[tid].violation())
            return true;
    }

    return false;
}

template<class Impl>
int
O3liteLSQ<Impl>::getCount()
{
    unsigned total = 0;

    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        total += getCount(tid);
    }

    return total;
}

template<class Impl>
int
O3liteLSQ<Impl>::numLoads()
{
    unsigned total = 0;

    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        total += numLoads(tid);
    }

    return total;
}

template<class Impl>
int
O3liteLSQ<Impl>::numStores()
{
    unsigned total = 0;

    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        total += thread[tid].numStores();
    }

    return total;
}

template<class Impl>
int
O3liteLSQ<Impl>::numLoadsReady()
{
    unsigned total = 0;

    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        total += thread[tid].numLoadsReady();
    }

    return total;
}

template<class Impl>
unsigned
O3liteLSQ<Impl>::numFreeEntries()
{
    unsigned total = 0;

    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        total += thread[tid].numFreeEntries();
    }

    return total;
}

template<class Impl>
unsigned
O3liteLSQ<Impl>::numFreeEntries(ThreadID tid)
{
    //if (lsqPolicy == Dynamic)
    //return numFreeEntries();
    //else
        return thread[tid].numFreeEntries();
}

template<class Impl>
bool
O3liteLSQ<Impl>::isFull()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (!(thread[tid].lqFull() || thread[tid].sqFull()))
            return false;
    }

    return true;
}

template<class Impl>
bool
O3liteLSQ<Impl>::isFull(ThreadID tid)
{
    //@todo: Change to Calculate All Entries for
    //Dynamic Policy
    if (lsqPolicy == Dynamic)
        return isFull();
    else
        return thread[tid].lqFull() || thread[tid].sqFull();
}

template<class Impl>
bool
O3liteLSQ<Impl>::lqFull()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (!thread[tid].lqFull())
            return false;
    }

    return true;
}

template<class Impl>
bool
O3liteLSQ<Impl>::lqFull(ThreadID tid)
{
    //@todo: Change to Calculate All Entries for
    //Dynamic Policy
    if (lsqPolicy == Dynamic)
        return lqFull();
    else
        return thread[tid].lqFull();
}

template<class Impl>
bool
O3liteLSQ<Impl>::sqFull()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (!sqFull(tid))
            return false;
    }

    return true;
}

template<class Impl>
bool
O3liteLSQ<Impl>::sqFull(ThreadID tid)
{
     //@todo: Change to Calculate All Entries for
    //Dynamic Policy
    if (lsqPolicy == Dynamic)
        return sqFull();
    else
        return thread[tid].sqFull();
}

template<class Impl>
bool
O3liteLSQ<Impl>::isStalled()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (!thread[tid].isStalled())
            return false;
    }

    return true;
}

template<class Impl>
bool
O3liteLSQ<Impl>::isStalled(ThreadID tid)
{
    if (lsqPolicy == Dynamic)
        return isStalled();
    else
        return thread[tid].isStalled();
}

template<class Impl>
bool
O3liteLSQ<Impl>::hasStoresToWB()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (hasStoresToWB(tid))
            return true;
    }

    return false;
}

template<class Impl>
bool
O3liteLSQ<Impl>::willWB()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (willWB(tid))
            return true;
    }

    return false;
}

template<class Impl>
void
O3liteLSQ<Impl>::dumpInsts()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        thread[tid].dumpInsts();
    }
}

template<class Impl>
bool
O3liteLSQ<Impl>::stBufFull()
{
    list<ThreadID>::iterator threads = activeThreads->begin();
    list<ThreadID>::iterator end = activeThreads->end();

    while (threads != end) {
        ThreadID tid = *threads++;

        if (!thread[tid].stBufFull())
            return false;
    }

    return true;
}

template<class Impl>
bool
O3liteLSQ<Impl>::stBufFull(ThreadID tid)
{
    //@todo: Change to Calculate All Entries for
    //Dynamic Policy
    if (lsqPolicy == Dynamic)
        return stBufFull();
    else
        return thread[tid].stBufFull();
}
