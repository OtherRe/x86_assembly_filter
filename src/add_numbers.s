SECTION .TEXT
	GLOBAL add_numbers

add_numbers:
	push ebp
	mov ebp, esp
	
	mov eax, [ebp + 8]
	mov ecx, [ebp + 12]
	add eax, ecx

	mov esp, ebp
	pop ebp
	ret
