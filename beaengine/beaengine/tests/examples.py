#!/usr/bin/python

# -*- coding: utf-8 -*-
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>
#
# @author : beaengine@gmail.com

from headers.BeaEnginePython import *
from nose.tools import *
import struct
import yaml

class TestExample:

    def setUp(self):
        Buffer = b'\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90'
        self.Target = create_string_buffer(Buffer,len(Buffer))

    def test_SimpleUseCase(self):
        Instruction = DISASM()
        Instruction.EIP = addressof(self.Target)
        Error = False
        for i in range(20):
            InstrLength = Disasm(addressof(Instruction))
            if InstrLength != UNKNOWN_OPCODE:
                Instruction.EIP = Instruction.EIP + InstrLength
            else :
                Error = True

        assert_equal(Error, False)


    def test_SecurityBlockUseCase(self):
        Instruction = DISASM()
        Instruction.EIP = addressof(self.Target)
        OutOfBlock = False
        EndOfSection =  Instruction.EIP + 10
        while not OutOfBlock:
            if Instruction.EIP < EndOfSection:
                Instruction.SecurityBlock = EndOfSection - Instruction.EIP
                InstrLength = Disasm(addressof(Instruction))
                if InstrLength != UNKNOWN_OPCODE and InstrLength != OUT_OF_BLOCK:
                    Instruction.EIP = Instruction.EIP + InstrLength
            else :
                OutOfBlock = True

        assert_equal(OutOfBlock, True)
        assert_equal(Instruction.EIP, EndOfSection)

    def test_VirtualAddrUseCase(self):
        myDisasm = DISASM()
        myDisasm.VirtualAddr = 0x401000
        Buffer = b'\xe9\x00\x00\x00\x00'
        Target = create_string_buffer(Buffer,len(Buffer))
        myDisasm.EIP = addressof(Target)
        InstrLength = Disasm(addressof(myDisasm))
        assert_equal(myDisasm.Instruction.AddrValue, 0x401005)

    def test_OptionsUseCase(self):
        myDisasm = DISASM()
        myDisasm.Options = NasmSyntax + PrefixedNumeral + ShowSegmentRegs
        Buffer = b'\x89\x94\x88\x00\x20\x40\x00 '
        Target = create_string_buffer(Buffer,len(Buffer))
        myDisasm.EIP = addressof(Target)
        InstrLength = Disasm(addressof(myDisasm))
        assert_equal(myDisasm.CompleteInstr, 'mov  [ds:eax+ecx*4+0x00402000], edx')



