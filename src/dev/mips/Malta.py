# Copyright (c) 2007 The Regents of The University of Michigan
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
# Authors: Korey Sewell

from m5.params import *
from m5.proxy import *

from BadDevice import BadDevice
from Device import BasicPioDevice
from MipsBackdoor import MipsBackdoor
from Pci import PciConfigAll
from Platform import Platform
from Uart import Uart8250

class MaltaCChip(BasicPioDevice):
    type = 'MaltaCChip'
    malta = Param.Malta(Parent.any, "Malta")

class MaltaIO(BasicPioDevice):
    type = 'MaltaIO'
    time = Param.Time('01/01/2009',
        "System time to use (0 for actual time, default is 1/1/06)")
    year_is_bcd = Param.Bool(False,
            "The RTC should interpret the year as a BCD value")
    malta = Param.Malta(Parent.any, "Malta")
    frequency = Param.Frequency('1024Hz', "frequency of interrupts")

class MaltaPChip(BasicPioDevice):
    type = 'MaltaPChip'
    malta = Param.Malta(Parent.any, "Malta")

class Malta(Platform):
    type = 'Malta'
    system = Param.System(Parent.any, "system")
    cchip = MaltaCChip(pio_addr=0x801a0000000)
    io = MaltaIO(pio_addr=0x801fc000000)
    uart = Uart8250(pio_addr=0xBFD003F8)
    backdoor = MipsBackdoor(pio_addr=0xBFD00F00, disk=Parent.simple_disk)

    # Attach I/O devices to specified bus object.  Can't do this
    # earlier, since the bus object itself is typically defined at the
    # System level.
    def attachIO(self, bus):
        self.cchip.pio = bus.port
        self.io.pio = bus.port
        self.uart.pio = bus.port
        self.backdoor.pio = bus.port
