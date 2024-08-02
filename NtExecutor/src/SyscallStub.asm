; ----------------------------------------------------------------------------------------------------------- 
; Data segment
; ----------------------------------------------------------------------------------------------------------- 
EXTERN _SysId: dword


; ----------------------------------------------------------------------------------------------------------- 
; Text or code segment
; ----------------------------------------------------------------------------------------------------------- 
_TEXT SEGMENT
 
; Needed windows functions 
EXTERN printf: PROC
 
; The PUBLIC modifier will make your function visible and callable outside
ALIGN 16
PUBLIC _SetSysid 
PUBLIC _NtWrapperArg

_SetSysid PROC

mov r15, rcx
ret
_SetSysid ENDP


_NtWrapperArg PROC
 mov eax, _SysId
 mov r10, rcx
 syscall
ret
_NtWrapperArg ENDP


_TEXT ENDS
 
END