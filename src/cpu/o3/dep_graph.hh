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
 * Authors: Kevin Lim
 */

#ifndef __CPU_O3_DEP_GRAPH_HH__
#define __CPU_O3_DEP_GRAPH_HH__

#include "cpu/o3/comm.hh"

/** Node in a linked list. */
template <class DynInstPtr>
class DependencyEntry
{
  public:
    DependencyEntry()
        : inst(NULL), next(NULL)
    { }

    DynInstPtr inst;
    //Might want to include data about what arch. register the
    //dependence is waiting on.
    DependencyEntry<DynInstPtr> *next;
};

/** Array of linked list that maintains the dependencies between
 * producing instructions and consuming instructions.  Each linked
 * list represents a single physical register, having the future
 * producer of the register's value, and all consumers waiting on that
 * value on the list.  The head node of each linked list represents
 * the producing instruction of that register.  Instructions are put
 * on the list upon reaching the IQ, and are removed from the list
 * either when the producer completes, or the instruction is squashed.
*/
template <class DynInstPtr>
class DependencyGraph
{
  public:
    typedef DependencyEntry<DynInstPtr> DepEntry;

    /** Default construction.  Must call resize() prior to use. */
    DependencyGraph()
        : numEntries(0), memAllocCounter(0), nodesTraversed(0), nodesRemoved(0)
    { }

    ~DependencyGraph();

    /** Resize the dependency graph to have num_entries registers. */
    void resize(int num_entries);

    /** Clears all of the linked lists. */
    void reset();

    /** Inserts an instruction to be dependent on the given index. */
    void insert(PhysRegIndex idx, DynInstPtr &new_inst);

    /** Sets the producing instruction of a given register. */
    void setInst(PhysRegIndex idx, DynInstPtr &new_inst)
    { dependGraph[idx].inst = new_inst; }

    /** Clears the producing instruction. */
    void clearInst(PhysRegIndex idx)
    { dependGraph[idx].inst = NULL; }

    /** Removes an instruction from a single linked list. */
    void remove(PhysRegIndex idx, DynInstPtr &inst_to_remove);

    /** Removes and returns the newest dependent of a specific register. */
    DynInstPtr pop(PhysRegIndex idx);

    /** Checks if there are any dependents on a specific register. */
    bool empty(PhysRegIndex idx) { return !dependGraph[idx].next; }

    inline bool overSubscribe(PhysRegIndex idx);
    void setMaxSubscribers(int num_subscribers) {maxSubscribers = num_subscribers;}

    /** Debugging function to dump out the dependency graph.
     */
    void dump();

  private:
    /** Array of linked lists.  Each linked list is a list of all the
     *  instructions that depend upon a given register.  The actual
     *  register's index is used to index into the graph; ie all
     *  instructions in flight that are dependent upon r34 will be
     *  in the linked list of dependGraph[34].
     */
    DepEntry *dependGraph;

    std::vector<int> numSubscribers;

    /** Number of linked lists; identical to the number of registers. */
    int numEntries;

    int maxSubscribers;

    // Debug variable, remove when done testing.
    unsigned memAllocCounter;

  public:
    // Debug variable, remove when done testing.
    uint64_t nodesTraversed;
    // Debug variable, remove when done testing.
    uint64_t nodesRemoved;
};

template <class DynInstPtr>
DependencyGraph<DynInstPtr>::~DependencyGraph()
{
    delete [] dependGraph;
}

template <class DynInstPtr>
void
DependencyGraph<DynInstPtr>::resize(int num_entries)
{
    numEntries = num_entries;
    dependGraph = new DepEntry[numEntries];
        numSubscribers.resize(num_entries);
        std::vector<int>::iterator it = numSubscribers.begin();
        std::vector<int>::iterator eit = numSubscribers.end();
        while (it != eit) {
                *it = 0;
                it ++;
        }
}

template <class DynInstPtr>
void
DependencyGraph<DynInstPtr>::reset()
{
    // Clear the dependency graph
    DepEntry *curr;
    DepEntry *prev;

    for (int i = 0; i < numEntries; ++i) {
        curr = dependGraph[i].next;

        while (curr) {
            memAllocCounter--;

            prev = curr;
            curr = prev->next;
            prev->inst = NULL;

            delete prev;
        }

        if (dependGraph[i].inst) {
            dependGraph[i].inst = NULL;
        }

        dependGraph[i].next = NULL;

      numSubscribers[i] = 0;
  }
}

template <class DynInstPtr>
void
DependencyGraph<DynInstPtr>::insert(PhysRegIndex idx, DynInstPtr &new_inst)
{
  //Add this new, dependent instruction at the head of the dependency
  //chain.

  // First create the entry that will be added to the head of the
  // dependency chain.
  DepEntry *new_entry = new DepEntry;
  new_entry->next = dependGraph[idx].next;
  new_entry->inst = new_inst;

  // Then actually add it to the chain.
  dependGraph[idx].next = new_entry;

  ++memAllocCounter;

  numSubscribers[idx] ++;
  assert(numSubscribers[idx] <= maxSubscribers);
}

template <class DynInstPtr>
bool 
DependencyGraph<DynInstPtr>::overSubscribe(PhysRegIndex idx)
{
  return (numSubscribers[idx]==maxSubscribers);
}


template <class DynInstPtr>
void
DependencyGraph<DynInstPtr>::remove(PhysRegIndex idx,
                                    DynInstPtr &inst_to_remove)
{
    DepEntry *prev = &dependGraph[idx];
    DepEntry *curr = dependGraph[idx].next;

    // Make sure curr isn't NULL.  Because this instruction is being
    // removed from a dependency list, it must have been placed there at
    // an earlier time.  The dependency chain should not be empty,
    // unless the instruction dependent upon it is already ready.
    if (curr == NULL) {
        return;
    }

    nodesRemoved++;

    // Find the instruction to remove within the dependency linked list.
    while (curr->inst != inst_to_remove) {
        prev = curr;
        curr = curr->next;
        nodesTraversed++;

        assert(curr != NULL);
    }

    // Now remove this instruction from the list.
    prev->next = curr->next;

    --memAllocCounter;

    // Could push this off to the destructor of DependencyEntry
    curr->inst = NULL;

    delete curr;

        assert(numSubscribers[idx] > 0);
        numSubscribers[idx]--;
}

template <class DynInstPtr>
DynInstPtr
DependencyGraph<DynInstPtr>::pop(PhysRegIndex idx)
{
    DepEntry *node;
    node = dependGraph[idx].next;
    DynInstPtr inst = NULL;
    if (node) {
        inst = node->inst;
        dependGraph[idx].next = node->next;
        node->inst = NULL;
        memAllocCounter--;
        delete node;
    }
        assert(numSubscribers[idx] > 0);
        numSubscribers[idx] --;
    return inst;
}

template <class DynInstPtr>
void
DependencyGraph<DynInstPtr>::dump()
{
    DepEntry *curr;

    for (int i = 0; i < numEntries; ++i)
    {
        curr = &dependGraph[i];

        if (curr->inst) {
            cprintf("dependGraph[%i]: producer: %#x [sn:%lli] consumer: ",
                    i, curr->inst->readPC(), curr->inst->seqNum);
        } else {
            cprintf("dependGraph[%i]: No producer. consumer: ", i);
        }

        while (curr->next != NULL) {
            curr = curr->next;

            cprintf("%#x [sn:%lli] ",
                    curr->inst->readPC(), curr->inst->seqNum);
        }

        cprintf("\n");
    }
    cprintf("memAllocCounter: %i\n", memAllocCounter);
}

#endif // __CPU_O3_DEP_GRAPH_HH__
