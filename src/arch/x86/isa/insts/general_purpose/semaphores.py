# Copyright (c) 2007 The Hewlett-Packard Development Company
# All rights reserved.
#
# Redistribution and use of this software in source and binary forms,
# with or without modification, are permitted provided that the
# following conditions are met:
#
# The software must be used only for Non-Commercial Use which means any
# use which is NOT directed to receiving any direct monetary
# compensation for, or commercial advantage from such use.  Illustrative
# examples of non-commercial use are academic research, personal study,
# teaching, education and corporate research & development.
# Illustrative examples of commercial use are distributing products for
# commercial advantage and providing services using the software for
# commercial advantage.
#
# If you wish to use this software or functionality therein that may be
# covered by patents for commercial use, please contact:
#     Director of Intellectual Property Licensing
#     Office of Strategy and Technology
#     Hewlett-Packard Company
#     1501 Page Mill Road
#     Palo Alto, California  94304
#
# Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.  Redistributions
# in binary form must reproduce the above copyright notice, this list of
# conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.  Neither the name of
# the COPYRIGHT HOLDER(s), HEWLETT-PACKARD COMPANY, nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.  No right of
# sublicense is granted herewith.  Derivatives of the software and
# output created using the software may be prepared, but only for
# Non-Commercial Uses.  Derivatives of the software may be shared with
# others provided: (i) the others agree to abide by the list of
# conditions herein which includes the Non-Commercial Use restrictions;
# and (ii) such Derivatives of the software include the above copyright
# notice to acknowledge the contribution from this software where
# applicable, this list of conditions and the disclaimer below.
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
# Authors: Gabe Black

microcode = '''
def macroop CMPXCHG_R_R {
    sub t0, rax, reg, flags=(OF, SF, ZF, AF, PF, CF)
    mov reg, reg, regm, flags=(CZF,)
    mov rax, rax, reg, flags=(nCZF,)
};

def macroop CMPXCHG_M_R {
    ldst t1, seg, sib, disp
    sub t0, rax, t1, flags=(OF, SF, ZF, AF, PF, CF)

    mov t1, t1, reg, flags=(CZF,)
    st t1, seg, sib, disp
    mov rax, rax, t1, flags=(nCZF,)
};

def macroop CMPXCHG_P_R {
    rdip t7
    ldst t1, seg, riprel, disp
    sub t0, rax, t1, flags=(OF, SF, ZF, AF, PF, CF)

    mov t1, t1, reg, flags=(CZF,)
    st t1, seg, riprel, disp
    mov rax, rax, t1, flags=(nCZF,)
};

def macroop CMPXCHG_LOCKED_M_R {
    ldstl t1, seg, sib, disp
    sub t0, rax, t1, flags=(OF, SF, ZF, AF, PF, CF)

    mov t1, t1, reg, flags=(CZF,)
    stul t1, seg, sib, disp
    mov rax, rax, t1, flags=(nCZF,)
};

def macroop CMPXCHG_LOCKED_P_R {
    rdip t7
    ldstl t1, seg, riprel, disp
    sub t0, rax, t1, flags=(OF, SF, ZF, AF, PF, CF)

    mov t1, t1, reg, flags=(CZF,)
    stul t1, seg, riprel, disp
    mov rax, rax, t1, flags=(nCZF,)
};

def macroop XADD_M_R {
    ldst t1, seg, sib, disp
    add t2, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t2, seg, sib, disp
    mov reg, reg, t1
};

def macroop XADD_P_R {
    rdip t7
    ldst t1, seg, riprel, disp
    add t2, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t2, seg, riprel, disp
    mov reg, reg, t1
};

def macroop XADD_LOCKED_M_R {
    ldstl t1, seg, sib, disp
    add t2, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t2, seg, sib, disp
    mov reg, reg, t1
};

def macroop XADD_LOCKED_P_R {
    rdip t7
    ldstl t1, seg, riprel, disp
    add t2, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t2, seg, riprel, disp
    mov reg, reg, t1
};

def macroop XADD_R_R {
    add t2, regm, reg, flags=(OF,SF,ZF,AF,PF,CF)
    mov regm, regm, reg
    mov reg, reg, t2
};

'''

cmpxchg8bCode = '''
def macroop CMPXCHG8B_%(suffix)s {
    %(rdip)s
    lea t1, seg, %(sib)s, disp, dataSize=asz
    ldst%(l)s t2, seg, [1, t0, t1], 0
    ldst%(l)s t3, seg, [1, t0, t1], dsz

    sub t0, rax, t2, flags=(ZF,)
    br label("doneComparing"), flags=(nCZF,)
    sub t0, rdx, t3, flags=(ZF,)
doneComparing:

    # If they're equal, set t3:t2 to rbx:rcx to write to memory
    mov t2, t2, rbx, flags=(CZF,)
    mov t3, t3, rcx, flags=(CZF,)

    # If they're not equal, set rdx:rax to the value from memory.
    mov rax, rax, t2, flags=(nCZF,)
    mov rdx, rdx, t3, flags=(nCZF,)

    # Write to memory
    st%(ul)s t3, seg, [1, t0, t1], dsz
    st%(ul)s t2, seg, [1, t0, t1], 0
};
'''

microcode += cmpxchg8bCode % {"rdip": "", "sib": "sib",
                              "l": "", "ul": "",
                              "suffix": "M"}
microcode += cmpxchg8bCode % {"rdip": "rdip t7", "sib": "riprel",
                              "l": "", "ul": "",
                              "suffix": "P"}
microcode += cmpxchg8bCode % {"rdip": "", "sib": "sib",
                              "l": "l", "ul": "ul",
                              "suffix": "LOCKED_M"}
microcode += cmpxchg8bCode % {"rdip": "rdip t7", "sib": "riprel",
                              "l": "l", "ul": "ul",
                              "suffix": "LOCKED_P"}

#let {{
#    class XCHG(Inst):
#       "GenFault ${new UnimpInstFault}"
#}};
