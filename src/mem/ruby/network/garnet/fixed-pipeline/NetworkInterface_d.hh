/*
 * Copyright (c) 2008 Princeton University
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
 * Authors: Niket Agarwal
 */

#ifndef NET_INTERFACE_D_H
#define NET_INTERFACE_D_H

#include <iostream>

#include "mem/ruby/network/garnet/NetworkHeader.hh"
#include "mem/ruby/network/garnet/fixed-pipeline/GarnetNetwork_d.hh"
#include "mem/gems_common/Vector.hh"
#include "mem/ruby/common/Consumer.hh"
#include "mem/ruby/slicc_interface/Message.hh"
#include "mem/ruby/network/garnet/fixed-pipeline/NetworkLink_d.hh"
#include "mem/ruby/network/garnet/fixed-pipeline/CreditLink_d.hh"
#include "mem/ruby/network/garnet/fixed-pipeline/OutVcState_d.hh"

class NetworkMessage;
class MessageBuffer;
class flitBuffer_d;

class NetworkInterface_d : public Consumer {
public:
        NetworkInterface_d(int id, int virtual_networks, GarnetNetwork_d* network_ptr);

        ~NetworkInterface_d();

        void addInPort(NetworkLink_d *in_link, CreditLink_d *credit_link);
        void addOutPort(NetworkLink_d *out_link, CreditLink_d *credit_link);

        void wakeup();
        void addNode(Vector<MessageBuffer *> &inNode, Vector<MessageBuffer *> &outNode);
        void printConfig(std::ostream& out) const;
        void print(std::ostream& out) const;
        int get_vnet(int vc);

private:
/**************Data Members*************/
        GarnetNetwork_d *m_net_ptr;
        int m_virtual_networks, m_num_vcs, m_vc_per_vnet;
        NodeID m_id;
        Vector<OutVcState_d *> m_out_vc_state;
        Vector<int > m_vc_allocator;
        int m_vc_round_robin; // For round robin scheduling
        flitBuffer_d *outSrcQueue; // For modelling link contention
        flitBuffer_d *creditQueue;

        NetworkLink_d *inNetLink;
        NetworkLink_d *outNetLink;
        CreditLink_d *m_credit_link;
        CreditLink_d *m_ni_credit_link;

        // Input Flit Buffers
        Vector<flitBuffer_d *>   m_ni_buffers; // The flit buffers which will serve the Consumer
        Vector<Time > m_ni_enqueue_time;

        Vector<MessageBuffer *> inNode_ptr; // The Message buffers that takes messages from the protocol
        Vector<MessageBuffer *> outNode_ptr; // The Message buffers that provides messages to the protocol

        bool flitisizeMessage(MsgPtr msg_ptr, int vnet);
        int calculateVC(int vnet);
        void scheduleOutputLink();
        void checkReschedule();
};

#endif
