# Copyright (c) 2006-2007 The Regents of The University of Michigan
# Copyright (c) 2010 Advanced Micro Devices, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors: Ron Dreslinski

import m5
from m5.objects import *
from m5.defines import buildEnv
from m5.util import addToPath
import os, optparse, sys

if buildEnv['FULL_SYSTEM']:
    panic("This script requires system-emulation mode (*_SE).")

# Get paths we might need
config_path = os.path.dirname(os.path.abspath(__file__))
config_root = os.path.dirname(config_path)
m5_root = os.path.dirname(config_root)
addToPath(config_root+'/configs/common')
addToPath(config_root+'/configs/ruby')
addToPath(config_root+'/configs/ruby/protocols')
addToPath(config_root+'/configs/ruby/topologies')

import Ruby

parser = optparse.OptionParser()

#
# Set the default cache size and associativity to be very small to encourage
# races between requests and writebacks.
#
parser.add_option("--l1d_size", type="string", default="256B")
parser.add_option("--l1i_size", type="string", default="256B")
parser.add_option("--l2_size", type="string", default="512B")
parser.add_option("--l1d_assoc", type="int", default=2)
parser.add_option("--l1i_assoc", type="int", default=2)
parser.add_option("--l2_assoc", type="int", default=2)

execfile(os.path.join(config_root, "configs/common", "Options.py"))

(options, args) = parser.parse_args()

#MAX CORES IS 8 with the fals sharing method
nb_cores = 8

# ruby does not support atomic, functional, or uncacheable accesses
cpus = [ MemTest(atomic=False, percent_functional=0, \
                 percent_uncacheable=0) \
         for i in xrange(nb_cores) ]

# overwrite options.num_cpus with the nb_cores value
options.num_cpus = nb_cores
 
# system simulated
system = System(cpu = cpus,
                funcmem = PhysicalMemory(),
                physmem = PhysicalMemory())

system.ruby = Ruby.create_system(options, system.physmem)

assert(len(cpus) == len(system.ruby.cpu_ruby_ports))

for (i, ruby_port) in enumerate(system.ruby.cpu_ruby_ports):
     #
     # Tie the cpu test and functional ports to the ruby cpu ports and
     # physmem, respectively
     #
     cpus[i].test = ruby_port.port
     cpus[i].functional = system.funcmem.port

# -----------------------
# run simulation
# -----------------------

root = Root(system = system)
root.system.mem_mode = 'timing'

# Not much point in this being higher than the L1 latency
m5.ticks.setGlobalFrequency('1ns')
