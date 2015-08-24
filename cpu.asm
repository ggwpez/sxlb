[BITS 32]
SECTION .text
[global _CPU_CPUID_supported]
[global _CPU_CPUID_request_00000000h]
[global _CPU_CPUID_request_00000001h]

_CPU_CPUID_supported:
	pushfd			;push EFLAGS to stack
	pop eax			;pop it to eax

	mov ecx, 1		;prepare 21. bit to be set
	shl ecx, 21
	
	or eax, ecx		;set it
	push eax		;move it to stack

	popfd			;copy 4 bytes from stack to EFLAGS
	pushfd			;get EFLAGS back

	pop eax			;move EFLAGS to eax
	shr eax, 21		;check for 21. bit
	and eax, 1		;"
ret

_CPU_CPUID_request_00000000h:
	push ebp		;set up stack frame
	mov ebp, esp	;"

	xor eax, eax
	 
	cpuid			;perform the request

	mov eax, dword [ebp+8]	;get string pointer

	mov [eax],		ebx
	mov [eax+4],	edx
	mov [eax+8],	ecx
	mov [eax+12],	dword 0	;string \0 termination

	mov esp, ebp    ;restore stack frame
	pop ebp			;"
ret

_CPU_CPUID_request_00000001h:
	push ebp		;set up stack frame
	mov ebp, esp	;"

	mov eax, 1
	cpuid

	mov ecx, dword [ebp+8]	;get string pointer

	mov [ecx],		eax
	mov [ecx+4],	ebx
	mov [ecx+8],	ecx
	mov [ecx+12],	edx

	mov esp, ebp    ;restore stack frame
	pop ebp			;"
ret
