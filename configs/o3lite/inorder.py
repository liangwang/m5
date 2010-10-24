import os
import optparse
import sys
from os.path import join as joinpath

import m5
from m5.defines import buildEnv
from m5.objects import *
from m5.util import addToPath, fatal

addToPath('../common')
import Simulation
import CacheConfig
from Caches import *


config_path =  os.path.dirname(os.path.abspath(__file__))
config_root = os.path.dirname(config_path)
m5_root = os.path.dirname(config_root)

parser = optparse.OptionParser()

execfile(os.path.join(config_root, "o3lite", "options.py"))

(options, args) = parser.parse_args()

execfile(os.path.join(config_root, "o3lite", "benchmarks.py"))

process = LiveProcess()
process.executable = options.cmd
process.cmd = [options.cmd] + options.options.split()

#system = System(cpu = [TimingSimpleCPU(cpu_id = i,
system = System(cpu = [InOrderCPU(cpu_id = i,
    clock = '2GHz') for i in xrange(options.num_cpus)],
    physmem = PhysicalMemory(range=AddrRange("2048MB")),
    membus = Bus(), mem_mode = "atomic")

system.physmem.port = system.membus.port

system.l2 = L2Cache(size='2MB')
system.tol2bus = Bus()
system.l2.cpu_side = system.tol2bus.port
system.l2.mem_side = system.membus.port
system.l2.num_cpus = options.num_cpus

for i in xrange (options.num_cpus):
    system.cpu[i].addPrivateSplitL1Caches(L1Cache(size='32kB'),
            L1Cache(size = '64kB'))
    system.cpu[i].connectMemPorts(system.tol2bus)



root = Root(system = system)

for i in xrange (options.num_cpus):
    if options.benchmark == 'FFT':
        system.cpu[i].workload = FFT()
    elif options.benchmark == 'Cholesky':
        system.cpu[i].workload = Cholesky()
    elif options.benchmark == 'LU_contig':
        system.cpu[i].workload = LU_contig()
    elif options.benchmark == 'LU_noncontig':
        system.cpu[i].workload = LU_noncontig()
    elif options.benchmark == 'Radix':
        system.cpu[i].workload = Radix()
    elif options.benchmark == 'Barnes':
        system.cpu[i].workload = Barnes()
    elif options.benchmark == 'FMM':
        system.cpu[i].workload = FMM()
    elif options.benchmark == 'OceanContig':
        system.cpu[i].workload = Ocean_contig()
    elif options.benchmark == 'OceanNoncontig':
        system.cpu[i].workload = Ocean_noncontig()
    elif options.benchmark == 'Raytrace':
        system.cpu[i].workload = Raytrace()
    elif options.benchmark == 'WaterNSquared':
        system.cpu[i].workload = Water_nsquared()
    elif options.benchmark == 'WaterSpatial':
        system.cpu[i].workload = Water_spatial()
    else:
        system.cpu[i].workload = process

root.system.mem_mode = 'timing'

m5.instantiate(root)

try:
    exit_event = m5.simulate(m5.MaxTick)
    print 'Exiting @ tick', m5.curTick(), 'because', exit_event.getCause()
except:
    print 'Failed @ tick', m5.curTick()
