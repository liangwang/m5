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
def macroop OR_R_R
{
    or reg, reg, regm, flags=(OF,SF,ZF,PF,CF)
};

def macroop OR_M_I
{
    limm t2, imm
    ldst t1, seg, sib, disp
    or t1, t1, t2, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, sib, disp
};

def macroop OR_P_I
{
    limm t2, imm
    rdip t7
    ldst t1, seg, riprel, disp
    or t1, t1, t2, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop OR_LOCKED_M_I
{
    limm t2, imm
    ldstl t1, seg, sib, disp
    or t1, t1, t2, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, sib, disp
};

def macroop OR_LOCKED_P_I
{
    limm t2, imm
    rdip t7
    ldstl t1, seg, riprel, disp
    or t1, t1, t2, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, riprel, disp
};

def macroop OR_M_R
{
    ldst t1, seg, sib, disp
    or t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, sib, disp
};

def macroop OR_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    or t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop OR_LOCKED_M_R
{
    ldstl t1, seg, sib, disp
    or t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, sib, disp
};

def macroop OR_LOCKED_P_R
{
    rdip t7
    ldstl t1, seg, riprel, disp
    or t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, riprel, disp
};

def macroop OR_R_M
{
    ld t1, seg, sib, disp
    or reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop OR_R_P
{
    rdip t7
    ld t1, seg, riprel, disp
    or reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop OR_R_I
{
    limm t1, imm
    or reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop XOR_R_R
{
    xor reg, reg, regm, flags=(OF,SF,ZF,PF,CF)
};

def macroop XOR_R_I
{
    limm t1, imm
    xor reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop XOR_M_I
{
    limm t2, imm
    ldst t1, seg, sib, disp
    xor t1, t1, t2, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, sib, disp
};

def macroop XOR_P_I
{
    limm t2, imm
    rdip t7
    ldst t1, seg, riprel, disp
    xor t1, t1, t2, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop XOR_LOCKED_M_I
{
    limm t2, imm
    ldstl t1, seg, sib, disp
    xor t1, t1, t2, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, sib, disp
};

def macroop XOR_LOCKED_P_I
{
    limm t2, imm
    rdip t7
    ldstl t1, seg, riprel, disp
    xor t1, t1, t2, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, riprel, disp
};

def macroop XOR_M_R
{
    ldst t1, seg, sib, disp
    xor t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, sib, disp
};

def macroop XOR_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    xor t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop XOR_LOCKED_M_R
{
    ldstl t1, seg, sib, disp
    xor t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, sib, disp
};

def macroop XOR_LOCKED_P_R
{
    rdip t7
    ldstl t1, seg, riprel, disp
    xor t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, riprel, disp
};

def macroop XOR_R_M
{
    ld t1, seg, sib, disp
    xor reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop XOR_R_P
{
    rdip t7
    ld t1, seg, riprel, disp
    xor reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop AND_R_R
{
    and reg, reg, regm, flags=(OF,SF,ZF,PF,CF)
};

def macroop AND_R_M
{
    ld t1, seg, sib, disp
    and reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop AND_R_P
{
    rdip t7
    ld t1, seg, riprel, disp
    and reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop AND_R_I
{
    limm t1, imm
    and reg, reg, t1, flags=(OF,SF,ZF,PF,CF)
};

def macroop AND_M_I
{
    ldst t2, seg, sib, disp
    limm t1, imm
    and t2, t2, t1, flags=(OF,SF,ZF,PF,CF)
    st t2, seg, sib, disp
};

def macroop AND_P_I
{
    rdip t7
    ldst t2, seg, riprel, disp
    limm t1, imm
    and t2, t2, t1, flags=(OF,SF,ZF,PF,CF)
    st t2, seg, riprel, disp
};

def macroop AND_LOCKED_M_I
{
    ldstl t2, seg, sib, disp
    limm t1, imm
    and t2, t2, t1, flags=(OF,SF,ZF,PF,CF)
    stul t2, seg, sib, disp
};

def macroop AND_LOCKED_P_I
{
    rdip t7
    ldstl t2, seg, riprel, disp
    limm t1, imm
    and t2, t2, t1, flags=(OF,SF,ZF,PF,CF)
    stul t2, seg, riprel, disp
};

def macroop AND_M_R
{
    ldst t1, seg, sib, disp
    and t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, sib, disp
};

def macroop AND_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    and t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop AND_LOCKED_M_R
{
    ldstl t1, seg, sib, disp
    and t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, sib, disp
};

def macroop AND_LOCKED_P_R
{
    rdip t7
    ldstl t1, seg, riprel, disp
    and t1, t1, reg, flags=(OF,SF,ZF,PF,CF)
    stul t1, seg, riprel, disp
};

def macroop NOT_R
{
    limm t1, -1
    xor reg, reg, t1
};

def macroop NOT_M
{
    limm t1, -1
    ldst t2, seg, sib, disp
    xor t2, t2, t1
    st t2, seg, sib, disp
};

def macroop NOT_P
{
    limm t1, -1
    rdip t7
    ldst t2, seg, riprel, disp
    xor t2, t2, t1
    st t2, seg, riprel, disp
};

def macroop NOT_LOCKED_M
{
    limm t1, -1
    ldstl t2, seg, sib, disp
    xor t2, t2, t1
    stul t2, seg, sib, disp
};

def macroop NOT_LOCKED_P
{
    limm t1, -1
    rdip t7
    ldstl t2, seg, riprel, disp
    xor t2, t2, t1
    stul t2, seg, riprel, disp
};
'''
