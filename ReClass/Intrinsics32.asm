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
;	32 bit
;
; --


.386
.MODEL flat, stdcall

;*********************************************************************
; .data section
.DATA


;*********************************************************************
; .text section
.CODE

;************************************
; UINT32 
; __stdcall
; ReadFS(
;	UINT32 Offset
; );
;************************************
ReadFS PROC, arg1:DWORD
	push ecx
	mov ecx, arg1
	assume fs:nothing
	mov eax, fs:[ecx]
	pop ecx
	ret
ReadFS ENDP

END
