.CODE

div64 PROC
	mov rax, rcx
	div r8
	ret
div64 ENDP

END