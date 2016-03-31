;- Simple Test BeaEngine 32-Bit, no Prototypes
;- "Helle" Klaus Helbing, 04.01.2010

;Header for BeaEngine 4.x (PureBasic 32 bits)

Structure REX_Struct
  W_.b
  R_.b
  X_.b
  B_.b
  state.b
EndStructure
Global Rex_Struct.REX_Struct

Structure PREFIXINFO
  Number.l
  NbUndefined.l
  LockPrefix.b
  OperandSize.b
  AddressSize.b
  RepnePrefix.b
  RepPrefix.b
  FSPrefix.b
  SSPrefix.b
  GSPrefix.b
  ESPrefix.b
  CSPrefix.b
  DSPrefix.b
  BranchTaken.b
  BranchNotTaken.b
  REX.REX_Struct
  AL1_.b                      ;alignment
  AL2_.b                      ;alignment
EndStructure
Global Prefixinfo.PREFIXINFO

Structure EFLStruct
  OF_.b                      ;(bit 11)
  SF_.b                      ;(bit 7)
  ZF_.b                      ;(bit 6)
  AF_.b                      ;(bit 4)
  PF_.b                      ;(bit 2)
  CF_.b                      ;(bit 0)
  TF_.b                      ;(bit 8)
  IF_.b                      ;(bit 9)
  DF_.b                      ;(bit 10)
  NT_.b                      ;(bit 14)
  RF_.b                      ;(bit 16)
  AL_.b                      ;alignment
EndStructure
Global Eflstruct.EFLStruct

Structure MEMORYTYPE
  BaseRegister.l
  IndexRegister.l
  Scale.l
  Displacement.q
EndStructure
Global Memorytype.MEMORYTYPE

Structure INSTRTYPE
  Category.l
  Opcode.l
  Mnemonic.b[24]
  BranchType.l
  Flags.EFLStruct
  AddrValue.q
  Immediat.q
  ImplicitModifiedRegs.l
EndStructure
Global Instrtype.INSTRTYPE

Structure ARGTYPE
  ArgMnemonic.b[24]
  ArgType.q
  ArgSize.l
  ArgPosition.l
  AccessMode.l
  Memory.MEMORYTYPE 
  SegmentReg.l
EndStructure
Global Argtype.ARGTYPE

Structure _Disasm
  EIP.l
  VirtualAddr.q
  SecurityBlock.l
  CompleteInstr.b[64]
  Archi.l
  Options.q
  Instruction.INSTRTYPE
  Argument1.ARGTYPE
  Argument2.ARGTYPE
  Argument3.ARGTYPE
  Argument4.ARGTYPE
  Prefix.PREFIXINFO
  Reserved_.l[44]
EndStructure
Global MyDisasm._Disasm

#LowPosition = 0
#HighPosition = 1

#ESReg = 1
#DSReg = 2
#FSReg = 3
#GSReg = 4
#CSReg = 5
#SSReg = 6

; ********** Prefixes
#InvalidPrefix      = 4
#InUsePrefix        = 1
#SuperfluousPrefix  = 2
#NotUsedPrefix      = 0
#MandatoryPrefix    = 8

; ********** EFLAGS states
#TE_ = 1                     ;test
#MO_ = 2                     ;modify
#RE_ = 4                     ;reset
#SE_ = 8                     ;set
#UN_ = 16                    ;undefined
#PR_ = 32                    ;restore prior value

; __________________________________________________________________________________________________________
;
;                                       INSTRUCTION_TYPE
; __________________________________________________________________________________________________________

#GENERAL_PURPOSE_INSTRUCTION = $00010000
#FPU_INSTRUCTION             = $00020000
#MMX_INSTRUCTION             = $00040000
#SSE_INSTRUCTION             = $00080000
#SSE2_INSTRUCTION            = $00100000
#SSE3_INSTRUCTION            = $00200000
#SSSE3_INSTRUCTION           = $00400000
#SSE41_INSTRUCTION           = $00800000
#SSE42_INSTRUCTION           = $01000000
#SYSTEM_INSTRUCTION          = $02000000
#VM_INSTRUCTION              = $04000000
UNDOCUMENTED_INSTRUCTION     = $08000000
AMD_INSTRUCTION              = $10000000
ILLEGAL_INSTRUCTION          = $20000000
AES_INSTRUCTION              = $40000000
CLMUL_INSTRUCTION            = $80000000
AVX_INSTRUCTION              = $100000000
AVX2_INSTRUCTION             = $200000000
MPX_INSTRUCTION              = $400000000

#DATA_TRANSFER               = 1
#ARITHMETIC_INSTRUCTION      = 2
#LOGICAL_INSTRUCTION         = 3
#SHIFT_ROTATE                = 4
#BIT_BYTE                    = 5
#CONTROL_TRANSFER            = 6
#STRING_INSTRUCTION          = 7
#InOutINSTRUCTION            = 8
#ENTER_LEAVE_INSTRUCTION     = 9
#FLAG_CONTROL_INSTRUCTION    = 10
#SEGMENT_REGISTER            = 11
#MISCELLANEOUS_INSTRUCTION   = 12

#COMPARISON_INSTRUCTION      = 13
#LOGARITHMIC_INSTRUCTION     = 14
#TRIGONOMETRIC_INSTRUCTION   = 15
#UNSUPPORTED_INSTRUCTION     = 16
   
#LOAD_CONSTANTS              = 17
#FPUCONTROL                  = 18
#STATE_MANAGEMENT            = 19

#CONVERSION_INSTRUCTION      = 20

#SHUFFLE_UNPACK              = 21
#PACKED_SINGLE_PRECISION     = 22
#SIMD128bits                 = 23
#SIMD64bits                  = 24
#CACHEABILITY_CONTROL        = 25
   
#FP_INTEGER_CONVERSION       = 26
#SPECIALIZED_128bits         = 27
#SIMD_FP_PACKED              = 28
#SIMD_FP_HORIZONTAL          = 29
#AGENT_SYNCHRONISATION       = 30

#PACKED_ALIGN_RIGHT          = 31 
#PACKED_SIGN                 = 32

; ****************************************** SSE4
   
#PACKED_BLENDING_INSTRUCTION = 33
#PACKED_TEST                 = 34
   
; CONVERSION_INSTRUCTION -> Packed Integer Format Conversions et Dword Packing With Unsigned Saturation
; COMPARISON -> Packed Comparison SIMD Integer Instruction
; ARITHMETIC_INSTRUCTION -> Dword Multiply Instruction
; DATA_TRANSFER -> POPCNT

#PACKED_MINMAX               = 35
#HORIZONTAL_SEARCH           = 36
#PACKED_EQUALITY             = 37
#STREAMING_LOAD              = 38
#INSERTION_EXTRACTION        = 39
#DOT_PRODUCT                 = 40
#SAD_INSTRUCTION             = 41
#ACCELERATOR_INSTRUCTION     = 42
#ROUND_INSTRUCTION           = 43

; __________________________________________________________________________________________________________
;
;                                       BranchTYPE
; __________________________________________________________________________________________________________

#Jo_                         = 1
#Jno_                        = -1
#Jc_                         = 2
#Jnc_                        = -2
#Je_                         = 3
#Jne_                        = -3
#Ja_                         = 4
#Jna_                        = -4
#Js_                         = 5
#Jns_                        = -5
#Jp_                         = 6
#Jnp_                        = -6
#Jl_                         = 7
#Jnl_                        = -7
#Jg_                         = 8
#Jng_                        = -8
#Jb_                         = 2
#Jnb_                        = -2
#Jecxz_                      = 10
#JmpType                     = 11
#CallType                    = 12
#RetType                     = 13

; __________________________________________________________________________________________________________
;
;                                       ARGUMENTS_TYPE
; __________________________________________________________________________________________________________

#NO_ARGUMENT                 = $10000000
#REGISTER_TYPE               = $20000000
#MEMORY_TYPE                 = $40000000
#CONSTANT_TYPE               = $80000000

#MMX_REG                     = $00010000
#GENERAL_REG                 = $00020000
#FPU_REG                     = $00040000
#SSE_REG                     = $00080000
#CR_REG                      = $00100000
#DR_REG                      = $00200000
#SPECIAL_REG                 = $00400000
#MEMORY_MANAGEMENT_REG       = $00800000       ; GDTR (REG0), LDTR (REG1), IDTR (REG2), TR (REG3)
#SEGMENT_REG                 = $01000000       ; ES (REG0), CS (REG1), SS (REG2), DS (REG3), FS (REG4), GS (REG5)
#AVX_REG                     = $02000000
#MPX_REG                     = $04000000

#RELATIVE_                   = $04000000
#ABSOLUTE_                   = $08000000

#Read                        = 1
#WRITE                       = 2
; ************ Regs
#REG0                        = 1   ; 30h
#REG1                        = 2   ; 31h
#REG2                        = 4   ; 32h
#REG3                        = 8   ; 33h
#REG4                        = $10 ; 34h
#REG5                        = $20 ; 35h
#REG6                        = $40 ; 36h
#REG7                        = $80 ; 37h
#REG8                        = $100; 38h
#REG9                        = $200; 39h
#REG10                       = $400    ; 3Ah
#REG11                       = $800    ; 3Bh
#REG12                       = $1000   ; 3Ch
#REG13                       = $2000   ; 3Dh
#REG14                       = $4000   ; 3Eh
#REG15                       = $8000   ; 3Fh

; ************ SPECIAL_REG
#UNKNOWN_OPCODE              = -1
#OUT_OF_BLOCK                = 0
#NoTabulation                = 0
#Tabulation                  = 1
#MasmSyntax                  = 0
#GoAsmSyntax                 = $100
#NasmSyntax                  = $200
#ATSyntax                    = $400
#IntrinsicMemSyntax          = $800
#PrefixedNumeral             = $10000
#SuffixedNumeral             = 0
#ShowSegmentRegs             = $01000000
;------- End Header
