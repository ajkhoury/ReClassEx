using System;
using System.Runtime.InteropServices;

namespace Bea
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class REX_Struct
    {
        public byte W_;
        public byte R_;
        public byte X_;
        public byte B_;
        public byte state;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class PrefixInfo
    {
        public int Number;
        public int NbUndefined;
        public byte LockPrefix;
        public byte OperandSize;
        public byte AddressSize;
        public byte RepnePrefix;
        public byte RepPrefix;
        public byte FSPrefix;
        public byte SSPrefix;
        public byte GSPrefix;
        public byte ESPrefix;
        public byte CSPrefix;
        public byte DSPrefix;
        public byte BranchTaken;
        public byte BranchNotTaken;
        public REX_Struct REX;
		[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
		public string alignment;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class EFLStruct
    {
        public byte OF_;
        public byte SF_;
        public byte ZF_;
        public byte AF_;
        public byte PF_;
        public byte CF_;
        public byte TF_;
        public byte IF_;
        public byte DF_;
        public byte NT_;
        public byte RF_;
        public byte alignment;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class MemoryType
    {
        public Int32 BaseRegister;
        public Int32 IndexRegister;
        public Int32 Scale;
        public Int64 Displacement;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class InstructionType
    {
        public Int32 Category;
        public Int32 Opcode;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 24)]
        public string Mnemonic;
        public Int32 BranchType;
        public EFLStruct Flags;
        public UInt64 AddrValue;
        public Int64 Immediat;
        public UInt32 ImplicitModifiedRegs;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class ArgumentType
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 24)]
        public string ArgMnemonic;
        public Int32 ArgType;
        public Int32 ArgSize;
		public Int32 ArgPosition;
        public UInt32 AccessMode;
        public MemoryType Memory;
        public UInt32 SegmentReg;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class Disasm
    {
        public IntPtr EIP;
        public UInt64 VirtualAddr;
        public UInt32 SecurityBlock;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string CompleteInstr;
        public UInt32 Archi;
        public UInt64 Options;
        public InstructionType Instruction;
        public ArgumentType Argument1;
        public ArgumentType Argument2;
        public ArgumentType Argument3;
        public ArgumentType Argument4;
        public PrefixInfo Prefix;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 44, ArraySubType = UnmanagedType.U4)]
        UInt32[] Reserved_;
    }
}
