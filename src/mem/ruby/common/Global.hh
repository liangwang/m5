/*
 * Copyright (c) 1999-2008 Mark D. Hill and David A. Wood
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
 */

#ifndef __MEM_RUBY_COMMON_GLOBAL_HH__
#define __MEM_RUBY_COMMON_GLOBAL_HH__

// external includes for all classes
#include "mem/ruby/common/TypeDefines.hh"
#include "mem/ruby/common/Debug.hh"

// simple type declarations
typedef Time LogicalTime;
typedef int64 Index;            // what the address bit ripper returns
typedef int word;               // one word of a cache line
typedef unsigned int uint;
typedef int SwitchID;
typedef int LinkID;

class RubyEventQueue;
extern RubyEventQueue* g_eventQueue_ptr;

class RubySystem;
extern RubySystem* g_system_ptr;

class Debug;
extern Debug* g_debug_ptr;

// FIXME: this is required by the contructor of Directory_Entry.hh.
// It can't go into slicc_util.hh because it opens a can of ugly worms
extern inline int max_tokens()
{
  return 1024;
}

#endif // __MEM_RUBY_COMMON_GLOBAL_HH__

