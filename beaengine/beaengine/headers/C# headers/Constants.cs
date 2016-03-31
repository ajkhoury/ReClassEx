
namespace Bea
{
    public class BeaConstants
    {
        public static int INSTRUCT_LENGTH = 64;

        public enum SegmentRegister : byte
        {
            ESReg = 1,
            DSReg = 2,
            FSReg = 3,
            GSReg = 4,
            CSReg = 5,
            SSReg = 6
        }

        public enum PrefixType : byte
        {
            NotUsedPrefix = 0,
            InUsePrefix = 1,
            SuperfluousPrefix = 2,
            InvalidPrefix = 4,
            MandatoryPrefix = 8
        }

        public enum InstructionType : uint
        {
            GENERAL_PURPOSE_INSTRUCTION = 0x10000,
            FPU_INSTRUCTION = 0x20000,
            MMX_INSTRUCTION = 0x40000,
            SSE_INSTRUCTION = 0x80000,
            SSE2_INSTRUCTION = 0x100000,
            SSE3_INSTRUCTION = 0x200000,
            SSSE3_INSTRUCTION = 0x400000,
            SSE41_INSTRUCTION = 0x800000,
            SSE42_INSTRUCTION = 0x1000000,
            SYSTEM_INSTRUCTION = 0x2000000,
            VM_INSTRUCTION = 0x4000000,
            UNDOCUMENTED_INSTRUCTION = 0x8000000,
            AMD_INSTRUCTION = 0x10000000,
            ILLEGAL_INSTRUCTION = 0x20000000,
            AES_INSTRUCTION = 0x40000000,
            CLMUL_INSTRUCTION = 0x80000000,
            AVX_INSTRUCTION               = (int)0x100000000,
            AVX2_INSTRUCTION               = (int)0x200000000,
            MPX_INSTRUCTION               = (int)0x400000000,

            DATA_TRANSFER = 0x1,
            ARITHMETIC_INSTRUCTION,
            LOGICAL_INSTRUCTION,
            SHIFT_ROTATE,
            BIT_UInt8,
            CONTROL_TRANSFER,
            STRING_INSTRUCTION,
            InOutINSTRUCTION,
            ENTER_LEAVE_INSTRUCTION,
            FLAG_CONTROL_INSTRUCTION,
            SEGMENT_REGISTER,
            MISCELLANEOUS_INSTRUCTION,
            COMPARISON_INSTRUCTION,
            LOGARITHMIC_INSTRUCTION,
            TRIGONOMETRIC_INSTRUCTION,
            UNSUPPORTED_INSTRUCTION,
            LOAD_CONSTANTS,
            FPUCONTROL,
            STATE_MANAGEMENT,
            CONVERSION_INSTRUCTION,
            SHUFFLE_UNPACK,
            PACKED_SINGLE_PRECISION,
            SIMD128bits,
            SIMD64bits,
            CACHEABILITY_CONTROL,
            FP_INTEGER_CONVERSION,
            SPECIALIZED_128bits,
            SIMD_FP_PACKED,
            SIMD_FP_HORIZONTAL,
            AGENT_SYNCHRONISATION,
            PACKED_ALIGN_RIGHT,
            PACKED_SIGN,
            PACKED_BLENDING_INSTRUCTION,
            PACKED_TEST,
            PACKED_MINMAX,
            HORIZONTAL_SEARCH,
            PACKED_EQUALITY,
            STREAMING_LOAD,
            INSERTION_EXTRACTION,
            DOT_PRODUCT,
            SAD_INSTRUCTION,
            ACCELERATOR_INSTRUCTION,
            ROUND_INSTRUCTION
        }

        public enum EFlagState : byte
        {
            TE_ = 1,
            MO_ = 2,
            RE_ = 4,
            SE_ = 8,
            UN_ = 0x10,
            PR_ = 0x20
        }

        public enum BranchType : short
        {
            JO = 1,
            JC,
            JE,
            JA,
            JS,
            JP,
            JL,
            JG,
            JB,
            JECXZ,
            JmpType,
            CallType,
            RetType,
            JNO = -1,
            JNC = -2,
            JNE = -3,
            JNA = -4,
            JNS = -5,
            JNP = -6,
            JNL = -7,
            JNG = -8,
            JNB = -9
        }

        public enum ArgumentType : uint
        {
            NO_ARGUMENT = 0x10000000,
            REGISTER_TYPE = 0x20000000,
            MEMORY_TYPE = 0x40000000,
            CONSTANT_TYPE = 0x80000000,

            MMX_REG = 0x10000,
            GENERAL_REG = 0x20000,
            FPU_REG = 0x40000,
            SSE_REG = 0x80000,
            CR_REG = 0x100000,
            DR_REG = 0x200000,
            SPECIAL_REG = 0x400000,
            MEMORY_MANAGEMENT_REG = 0x800000,
            SEGMENT_REG = 0x1000000,
            AVX_REG = 0x2000000,
            MPX_REG = 0x4000000,

            RELATIVE_ = 0x4000000,
            ABSOLUTE_ = 0x8000000,

            READ = 0x1,
            WRITE = 0x2,

            REG0 = 0x1,
            REG1 = 0x2,
            REG2 = 0x4,
            REG3 = 0x8,
            REG4 = 0x10,
            REG5 = 0x20,
            REG6 = 0x40,
            REG7 = 0x80,
            REG8 = 0x100,
            REG9 = 0x200,
            REG10 = 0x400,
            REG11 = 0x800,
            REG12 = 0x1000,
            REG13 = 0x2000,
            REG14 = 0x4000,
            REG15 = 0x8000
        }

        public enum SpecialInfo : int
        {
            UNKNOWN_OPCODE = -1,
            OUT_OF_BLOCK = 0,

            /* === mask = 0xff */
            NoTabulation = 0x00000000,
            Tabulation = 0x00000001,

            /* === mask = 0xff00 */
            MasmSyntax = 0x00000000,
            GoAsmSyntax = 0x00000100,
            NasmSyntax = 0x00000200,
            ATSyntax = 0x00000400,
            IntrinsicMemSyntax= 0x00000800,

            /* === mask = 0xff0000 */
            PrefixedNumeral = 0x00010000,
            SuffixedNumeral = 0x00000000,

            /* === mask = 0xff000000 */
            ShowSegmentRegs = 0x01000000,
			
			LowPosition = 0,
			HighPosition = 1
        }
    }
}
