; *****************************************
;
;
;   Header for BeaEngine 4.x (fasm 32 bits)
;
;
; *****************************************

struct REX_Struct
    W_      db    0
    R_      db    0
    X_      db    0
    B_      db    0
    state   db    0
ends



struct PREFIXINFO
  Number          dd   0
  NbUndefined     dd   0
  LockPrefix      db    0
  OperandSize     db    0
  AddressSize     db    0
  RepnePrefix     db    0
  RepPrefix       db    0
  FSPrefix        db    0
  SSPrefix        db    0
  GSPrefix        db    0
  ESPrefix        db    0
  CSPrefix        db    0
  DSPrefix        db    0
  BranchTaken     db    0
  BranchNotTaken  db    0
  REX             REX_Struct
				  db    0 ; alignment
				  db    0 ; alignment
ends



struct EFLStruct
    OF_     db    0 ;(bit 11)
    SF_     db    0 ;(bit 7)
    ZF_     db    0 ;(bit 6)
    AF_     db    0 ;(bit 4)
    PF_     db    0 ;(bit 2)
    CF_     db    0 ;(bit 0)
    TF_     db    0 ;(bit 8)
    IF_     db    0 ;(bit 9)
    DF_     db    0 ;(bit 10)
    NT_     db    0 ;(bit 14)
    RF_     db    0 ;(bit 16)
            db    0 ; alignment
ends


struct  MEMORYTYPE
   BaseRegister    dd   0
   IndexRegister   dd   0
   Scale           dd   0
   Displacement    dq   0 
ends


struct INSTRTYPE
    Category                dd   0
    Opcode                  dd   0
    Mnemonic                db   24 dup(0)
    BranchType              dd   0
    Flags                   EFLStruct
    AddrValue               dq   0
    Immediat                dq   0
    ImplicitModifiedRegs    dd   0
ends


struct ARGTYPE
    ArgMnemonic     db   24 dup(0)
    ArgType         dq   0
    ArgSize         dd   0
	ArgPosition		dd	 0
    AccessMode      dd   0
    Memory          MEMORYTYPE
    SegmentReg      dd   0
ends


; ************************************** Define structure
struct _Disasm
    EIP             dd   0
    VirtualAddr     dq   0
    SecurityBlock   dd   0
    CompleteInstr   db   64 dup(0)
    Archi           dd   0
    Options         dq   0
    Instruction     INSTRTYPE
    Argument1       ARGTYPE
    Argument2       ARGTYPE
    Argument3       ARGTYPE
    Argument4       ARGTYPE
    Prefix          PREFIXINFO
    Reserved_       dd   44 dup(0)
ends

	LowPosition		equ		0
	HighPosition	equ		1

    ESReg   equ 1
    DSReg   equ 2
    FSReg   equ 3
    GSReg   equ 4
    CSReg   equ 5
    SSReg   equ 6


 ; ********** Prefixes

    InvalidPrefix               equ 4
    InUsePrefix                 equ 1
    SuperfluousPrefix           equ 2
    NotUsedPrefix               equ 0
    MandatoryPrefix             equ 8

 ; ********** EFLAGS states
 
    TE_      equ 1   ; test
    MO_      equ 2   ; modify
    RE_      equ 4   ; reset
    SE_      equ 8   ; set
    UN_      equ 10h   ; undefined
    PR_      equ 20h   ; restore prior value


; __________________________________________________________________________________________________________
;
;                                       INSTRUCTION_TYPE
; __________________________________________________________________________________________________________

    GENERAL_PURPOSE_INSTRUCTION equ 00010000h
    FPU_INSTRUCTION             equ 00020000h
    MMX_INSTRUCTION             equ 00040000h
    SSE_INSTRUCTION             equ 00080000h
    SSE2_INSTRUCTION            equ 00100000h
    SSE3_INSTRUCTION            equ 00200000h
    SSSE3_INSTRUCTION           equ 00400000h
    SSE41_INSTRUCTION           equ 00800000h
    SSE42_INSTRUCTION           equ 01000000h
    SYSTEM_INSTRUCTION          equ 02000000h
    VM_INSTRUCTION              equ 04000000h
    UNDOCUMENTED_INSTRUCTION    equ 08000000h
    AMD_INSTRUCTION             equ 10000000h
    ILLEGAL_INSTRUCTION         equ 20000000h
    AES_INSTRUCTION             equ 40000000h
    CLMUL_INSTRUCTION           equ 80000000h
    AVX_INSTRUCTION             equ 100000000h
    AVX2_INSTRUCTION            equ 200000000h
    MPX_INSTRUCTION             equ 400000000h

    DATA_TRANSFER               equ 1
    ARITHMETIC_INSTRUCTION      equ 2
    LOGICAL_INSTRUCTION         equ 3
    SHIFT_ROTATE                equ 4
    BIT_BYTE                    equ 5
    CONTROL_TRANSFER            equ 6
    STRING_INSTRUCTION          equ 7
    InOutINSTRUCTION            equ 8
    ENTER_LEAVE_INSTRUCTION     equ 9
    FLAG_CONTROL_INSTRUCTION    equ 10
    SEGMENT_REGISTER            equ 11
    MISCELLANEOUS_INSTRUCTION   equ 12

    COMPARISON_INSTRUCTION      equ 13
    LOGARITHMIC_INSTRUCTION     equ 14
    TRIGONOMETRIC_INSTRUCTION   equ 15
    UNSUPPORTED_INSTRUCTION     equ 16
    
    LOAD_CONSTANTS              equ 17
    FPUCONTROL                  equ 18
    STATE_MANAGEMENT            equ 19

    CONVERSION_INSTRUCTION      equ 20

    SHUFFLE_UNPACK              equ 21
    PACKED_SINGLE_PRECISION     equ 22
    SIMD128bits                 equ 23
    SIMD64bits                  equ 24
    CACHEABILITY_CONTROL        equ 25
    
    FP_INTEGER_CONVERSION       equ 26 
    SPECIALIZED_128bits         equ 27
    SIMD_FP_PACKED              equ 28
    SIMD_FP_HORIZONTAL          equ 29 
    AGENT_SYNCHRONISATION       equ 30

    PACKED_ALIGN_RIGHT          equ 31  
    PACKED_SIGN                 equ 32

    ; ****************************************** SSE4
    
    PACKED_BLENDING_INSTRUCTION equ 33
    PACKED_TEST                 equ 34
    
    ; CONVERSION_INSTRUCTION -> Packed Integer Format Conversions et Dword Packing With Unsigned Saturation
    ; COMPARISON -> Packed Comparison SIMD Integer Instruction
    ; ARITHMETIC_INSTRUCTION -> Dword Multiply Instruction
    ; DATA_TRANSFER -> POPCNT

    PACKED_MINMAX               equ 35
    HORIZONTAL_SEARCH           equ 36
    PACKED_EQUALITY             equ 37
    STREAMING_LOAD              equ 38
    INSERTION_EXTRACTION        equ 39
    DOT_PRODUCT                 equ 40
    SAD_INSTRUCTION             equ 41
    ACCELERATOR_INSTRUCTION     equ 42
    ROUND_INSTRUCTION           equ 43

; __________________________________________________________________________________________________________
;
;                                       BranchTYPE
; __________________________________________________________________________________________________________

    Jo_                         equ 1
    Jno_                        equ -1
    Jc_                         equ 2
    Jnc_                        equ -2
    Je_                         equ 3
    Jne_                        equ -3
    Ja_                         equ 4
    Jna_                        equ -4
    Js_                         equ 5
    Jns_                        equ -5
    Jp_                         equ 6
    Jnp_                        equ -6
    Jl_                         equ 7
    Jnl_                        equ -7
    Jg_                         equ 8
    Jng_                        equ -8
    Jb_                         equ 2
    Jnb_                        equ -2
    Jecxz_                      equ 10
    JmpType                     equ 11
    CallType                    equ 12
    RetType                     equ 13

; __________________________________________________________________________________________________________
;
;                                       ARGUMENTS_TYPE
; __________________________________________________________________________________________________________

    NO_ARGUMENT                 equ 10000000h
    REGISTER_TYPE               equ 20000000h
    MEMORY_TYPE                 equ 40000000h
    CONSTANT_TYPE               equ 80000000h

    MMX_REG                     equ 00010000h
    GENERAL_REG                 equ 00020000h
    FPU_REG                     equ 00040000h
    SSE_REG                     equ 00080000h
    CR_REG                      equ 00100000h
    DR_REG                      equ 00200000h
    SPECIAL_REG                 equ 00400000h
    MEMORY_MANAGEMENT_REG       equ 00800000h       ; GDTR (REG0), LDTR (REG1), IDTR (REG2), TR (REG3)
    SEGMENT_REG                 equ 01000000h       ; ES (REG0), CS (REG1), SS (REG2), DS (REG3), FS (REG4), GS (REG5)
    AVX_REG                     equ 02000000h
    MPX_REG                     equ 04000000h


    RELATIVE_                   equ 04000000h
    ABSOLUTE_                   equ 08000000h

    READ                        equ 1
    WRITE                       equ 2
    ; ************ Regs

    REG0                        equ 1   ; 30h
    REG1                        equ 2   ; 31h
    REG2                        equ 4   ; 32h
    REG3                        equ 8   ; 33h
    REG4                        equ 10h ; 34h
    REG5                        equ 20h ; 35h
    REG6                        equ 40h ; 36h
    REG7                        equ 80h ; 37h
    REG8                        equ 100h; 38h
    REG9                        equ 200h; 39h
    REG10                       equ 400h    ; 3Ah
    REG11                       equ 800h    ; 3Bh
    REG12                       equ 1000h   ; 3Ch
    REG13                       equ 2000h   ; 3Dh
    REG14                       equ 4000h   ; 3Eh
    REG15                       equ 8000h   ; 3Fh

    ; ************ SPECIAL_REG

    UNKNOWN_OPCODE              equ -1
    OUT_OF_BLOCK                equ 0
    NoTabulation                equ 0
    Tabulation                  equ 1
    MasmSyntax                  equ 0
    GoAsmSyntax                 equ 100h
    NasmSyntax                  equ 200h
    ATSyntax                    equ 400h
    IntrinsicMemSyntax          equ 800h
    PrefixedNumeral             equ 10000h
    SuffixedNumeral             equ 0
    ShowSegmentRegs             equ 01000000h

