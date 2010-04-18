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

#ifndef SWITCH_D_H
#define SWITCH_D_H

#include <iostream>

#include "mem/ruby/network/garnet/NetworkHeader.hh"
#include "mem/ruby/common/Consumer.hh"
#include "mem/ruby/network/garnet/fixed-pipeline/flitBuffer_d.hh"

class Router_d;
class OutputUnit_d;

class Switch_d : public Consumer {
public:
        Switch_d(Router_d *router);
        ~Switch_d();
        void wakeup();
        void init();
        void check_for_wakeup();
        void print(std::ostream& out) const {};
        inline void update_sw_winner(int inport, flit_d *t_flit)
        {
                m_switch_buffer[inport]->insert(t_flit);
        }
        inline double get_crossbar_count()
        {
                return m_crossbar_activity;
        }


private:
        int m_num_vcs;
        int m_num_inports;
        double m_crossbar_activity;
        Router_d *m_router;
        Vector<flitBuffer_d *> m_switch_buffer;
        Vector<OutputUnit_d *> m_output_unit;
};

#endif
