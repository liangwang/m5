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

#ifndef __ARCH_X86_PAGETABLE_HH__
#define __ARCH_X86_PAGETABLE_HH__

#include <iostream>
#include <string>

#include "base/bitunion.hh"
#include "base/misc.hh"
#include "base/types.hh"

class Checkpoint;

namespace X86ISA
{
    BitUnion64(VAddr)
        Bitfield<20, 12> longl1;
        Bitfield<29, 21> longl2;
        Bitfield<38, 30> longl3;
        Bitfield<47, 39> longl4;

        Bitfield<20, 12> pael1;
        Bitfield<29, 21> pael2;
        Bitfield<31, 30> pael3;

        Bitfield<21, 12> norml1;
        Bitfield<31, 22> norml2;
    EndBitUnion(VAddr)

    struct TlbEntry
    {
        // The base of the physical page.
        Addr paddr;

        // The beginning of the virtual page this entry maps.
        Addr vaddr;
        // The size of the page this entry represents.
        Addr size;

        // Read permission is always available, assuming it isn't blocked by
        // other mechanisms.
        bool writable;
        // Whether this page is accesible without being in supervisor mode.
        bool user;
        // Whether to use write through or write back. M5 ignores this and
        // lets the caches handle the writeback policy.
        //bool pwt;
        // Whether the page is cacheable or not.
        bool uncacheable;
        // Whether or not to kick this page out on a write to CR3.
        bool global;
        // A bit used to form an index into the PAT table.
        bool patBit;
        // Whether or not memory on this page can be executed.
        bool noExec;

        TlbEntry(Addr asn, Addr _vaddr, Addr _paddr);
        TlbEntry() {}

        void
        updateVaddr(Addr new_vaddr)
        {
            vaddr = new_vaddr;
        }

        Addr pageStart()
        {
            return paddr;
        }

        void serialize(std::ostream &os);
        void unserialize(Checkpoint *cp, const std::string &section);
    };
}

#endif
