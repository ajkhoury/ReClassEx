; ++
; 
; File:
;
;	intrinsics.asm
;
; Author:
;
;	Dude719
;
; Environment:
;
;	64 bit
;
; --

;*********************************************************************
; .data section
.DATA

;*********************************************************************
; .text section
.CODE

;************************************
; UINT64 
; ReadGS(
;	UINT64 Offset
; );
;************************************
ReadGS PROC
	mov rax, gs:[rcx]
	ret
ReadGS ENDP

END
