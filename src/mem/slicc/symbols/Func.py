# Copyright (c) 1999-2008 Mark D. Hill and David A. Wood
# Copyright (c) 2009 The Hewlett-Packard Development Company
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

from slicc.symbols.Symbol import Symbol
from slicc.symbols.Type import Type

class Func(Symbol):
    def __init__(self, table, ident, location, return_type, param_types,
                 param_strings, body, pairs, machine):
        super(Func, self).__init__(table, ident, location, pairs)
        self.return_type = return_type
        self.param_types = param_types
        self.param_strings = param_strings
        self.body = body
        self.isInternalMachineFunc = False

        if machine is None:
            self.c_ident = ident
        elif "external" in self or "primitive" in self:
            self.c_ident = ident
        else:
            self.machineStr = str(machine)
            # Append with machine name
            self.c_ident = "%s_%s" % (self.machineStr, ident)
            self.isInternalMachineFunc = True

    def __repr__(self):
        return ""

    @property
    def prototype(self):
        if "external" in self:
            return ""

        return_type = self.return_type.c_ident
        void_type = self.symtab.find("void", Type)
        if "return_by_ref" in self and self.return_type != void_type:
            return_type += "&"

        return "%s %s(%s);" % (return_type, self.c_ident,
                               ", ".join(self.param_strings))

    def writeCodeFiles(self, path):
        '''This write a function of object Chip'''
        if "external" in self:
            return

        code = self.symtab.codeFormatter()

        # Header
        code('''
/** Auto generated C++ code started by $__file__:$__line__ */

#include "mem/protocol/Types.hh"
''')

        if self.isInternalMachineFunc:
            code('#include "mem/protocol/${{self.machineStr}}_Controller.hh"')

        code('using namespace std;')
        # Generate function header
        void_type = self.symtab.find("void", Type)
        return_type = self.return_type.c_ident
        if "return_by_ref" in self and self.return_type != void_type:
            return_type += "&"

        if self.isInternalMachineFunc:
            klass = "%s_Controller" % self.machineStr
        else:
            klass = "Chip"

        params = ', '.join(self.param_strings)

        code('''
$return_type
${klass}::${{self.c_ident}}($params)
{
${{self.body}}
}
''')
        code.write(path, "%s.cc" % self.c_ident)

__all__ = [ "Func" ]
