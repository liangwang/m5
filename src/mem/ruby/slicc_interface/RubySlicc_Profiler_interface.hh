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

/*
 * These are the functions that exported to slicc from ruby.
 */

#ifndef __MEM_RUBY_SLICC_INTERFACE_RUBYSLICC_PROFILER_INTERFACE_HH__
#define __MEM_RUBY_SLICC_INTERFACE_RUBYSLICC_PROFILER_INTERFACE_HH__

#include <string>

#include "mem/protocol/AccessType.hh"
#include "mem/protocol/Directory_State.hh"
#include "mem/protocol/GenericRequestType.hh"
#include "mem/protocol/L1Cache_State.hh"
#include "mem/ruby/common/Address.hh"
#include "mem/ruby/common/Global.hh"
#include "mem/ruby/profiler/Profiler.hh"
#include "mem/ruby/system/NodeID.hh"

class Set;

void profile_request(int cache_state, Directory_State directory_state,
                     GenericRequestType request_type);
void profile_outstanding_persistent_request(int outstanding);
void profile_outstanding_request(int outstanding);
void profile_sharing(const Address& addr, AccessType type, NodeID requestor,
                     const Set& sharers, const Set& owner);
void profile_request(const std::string& L1CacheStateStr,
                     const std::string& L2CacheStateStr,
                     const std::string& directoryStateStr,
                     const std::string& requestTypeStr);
void profile_miss(const CacheMsg& msg, NodeID id);
void profile_L1Cache_miss(const CacheMsg& msg, NodeID id);
void profile_L2Cache_miss(GenericRequestType requestType, AccessModeType type,
                          int msgSize, PrefetchBit pfBit, NodeID l2cacheID);
void profile_token_retry(const Address& addr, AccessType type, int count);
void profile_filter_action(int action);
void profile_persistent_prediction(const Address& addr, AccessType type);
void profile_average_latency_estimate(int latency);
void profileMsgDelay(int virtualNetwork, int delayCycles);

void profile_multicast_retry(const Address& addr, int count);
void profileGetX(const Address& datablock, const Address& PC, const Set& owner,
                 const Set& sharers, NodeID requestor);
void profileGetS(const Address& datablock, const Address& PC, const Set& owner,
                 const Set& sharers, NodeID requestor);

void profileOverflow(const Address & addr, MachineID mach);

#endif // __MEM_RUBY_SLICC_INTERFACE_RUBYSLICC_PROFILER_INTERFACE_HH__
