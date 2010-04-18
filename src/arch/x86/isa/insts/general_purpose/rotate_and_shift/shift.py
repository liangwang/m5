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
def macroop SAL_R_I
{
    slli reg, reg, imm, flags=(CF,OF,SF,ZF,PF)
};

def macroop SAL_M_I
{
    ldst t1, seg, sib, disp
    slli t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SAL_P_I
{
    rdip t7
    ldst t1, seg, riprel, disp
    slli t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SAL_1_R
{
    slli reg, reg, 1, flags=(CF,OF,SF,ZF,PF)
};

def macroop SAL_1_M
{
    ldst t1, seg, sib, disp
    slli t1, t1, 1, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SAL_1_P
{
    rdip t7
    ldst t1, seg, riprel, disp
    slli t1, t1, 1, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SAL_R_R
{
    sll reg, reg, regm, flags=(CF,OF,SF,ZF,PF)
};

def macroop SAL_M_R
{
    ldst t1, seg, sib, disp
    sll t1, t1, reg, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SAL_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    sll t1, t1, reg, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SHLD_R_R
{
    mdbi regm, 0
    sld reg, reg, rcx, flags=(CF,OF,SF,ZF,PF)
};

def macroop SHLD_M_R
{
    ldst t1, seg, sib, disp
    mdbi reg, 0
    sld t1, t1, rcx, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SHLD_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    mdbi reg, 0
    sld t1, t1, rcx, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SHLD_R_R_I
{
    mdbi regm, 0
    sldi reg, reg, imm, flags=(CF,OF,SF,ZF,PF)
};

def macroop SHLD_M_R_I
{
    ldst t1, seg, sib, disp
    mdbi reg, 0
    sldi t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SHLD_P_R_I
{
    rdip t7
    ldst t1, seg, riprel, disp
    mdbi reg, 0
    sldi t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SHR_R_I
{
    srli reg, reg, imm, flags=(CF,OF,SF,ZF,PF)
};

def macroop SHR_M_I
{
    ldst t1, seg, sib, disp
    srli t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SHR_P_I
{
    rdip t7
    ldst t1, seg, riprel, disp
    srli t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SHR_1_R
{
    srli reg, reg, 1, flags=(CF,OF,SF,ZF,PF)
};

def macroop SHR_1_M
{
    ldst t1, seg, sib, disp
    srli t1, t1, 1, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SHR_1_P
{
    rdip t7
    ldst t1, seg, riprel, disp
    srli t1, t1, 1, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SHR_R_R
{
    srl reg, reg, regm, flags=(CF,OF,SF,ZF,PF)
};

def macroop SHR_M_R
{
    ldst t1, seg, sib, disp
    srl t1, t1, reg, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SHR_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    srl t1, t1, reg, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SHRD_R_R
{
    mdbi regm, 0
    srd reg, reg, rcx, flags=(CF,OF,SF,ZF,PF)
};

def macroop SHRD_M_R
{
    ldst t1, seg, sib, disp
    mdbi reg, 0
    srd t1, t1, rcx, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SHRD_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    mdbi reg, 0
    srd t1, t1, rcx, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SHRD_R_R_I
{
    mdbi regm, 0
    srdi reg, reg, imm, flags=(CF,OF,SF,ZF,PF)
};

def macroop SHRD_M_R_I
{
    ldst t1, seg, sib, disp
    mdbi reg, 0
    srdi t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SHRD_P_R_I
{
    rdip t7
    ldst t1, seg, riprel, disp
    mdbi reg, 0
    srdi t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SAR_R_I
{
    srai reg, reg, imm, flags=(CF,OF,SF,ZF,PF)
};

def macroop SAR_M_I
{
    ldst t1, seg, sib, disp
    srai t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SAR_P_I
{
    rdip t7
    ldst t1, seg, riprel, disp
    srai t1, t1, imm, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SAR_1_R
{
    srai reg, reg, 1, flags=(CF,OF,SF,ZF,PF)
};

def macroop SAR_1_M
{
    ldst t1, seg, sib, disp
    srai t1, t1, 1, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SAR_1_P
{
    rdip t7
    ldst t1, seg, riprel, disp
    srai t1, t1, 1, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};

def macroop SAR_R_R
{
    sra reg, reg, regm, flags=(CF,OF,SF,ZF,PF)
};

def macroop SAR_M_R
{
    ldst t1, seg, sib, disp
    sra t1, t1, reg, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, sib, disp
};

def macroop SAR_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    sra t1, t1, reg, flags=(CF,OF,SF,ZF,PF)
    st t1, seg, riprel, disp
};
'''
