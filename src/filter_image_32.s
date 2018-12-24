SECTION .TEXT
	GLOBAL filter_image

filter_image:
	push ebp
	mov ebp, esp
	sub esp, 12
	push esi
	push edi
	push ebx
	
	mov eax, 9
	mov edx, [ebp + 16]; kernel adress
	xor ecx, ecx
kernel_sum_loop: 
	add  ecx,  DWORD [edx]

	add  edx, 4
	dec  eax
	test eax, eax
	jnz  kernel_sum_loop
	
	mov eax, 1 ;make sure eax is greater than zero
	test ecx, ecx
	cmovz ecx, eax

	mov [ebp - 4], ecx
	
	mov eax, [ebp + 20]
	lea eax, [eax + 2 * eax] ;width in bytes
	mov [ebp - 12], eax
	
	mov ecx, [ebp + 24]
	lea ecx, [ecx - 2]
	mul ecx

	mov [ebp - 8],  eax
	mov ebx, DWORD [ebp - 12]
	sub DWORD [ebp + 12], ebx
	
	;[ebp - 4] -----> kernel sum
	;[ebp - 8] -----> number of bytes to process	
	;[ebp - 12] -----> width in bytes

	;eax ------> used to multiply
	;edx ------> sum accumulator
	;ecx ------> kernel pointer
	;esi  ------> loop counter
	;edi  ------> image pointer
	;ebx ------> width in bytes
		
filter_loop:
	xor   ebx, ebx        ;reset accumulator
	mov   ecx, [ebp + 16] ;reset kernel pointer
	mov   esi, 3	      ;reset loop counter
	
	mov edi, [ebp + 12]
	sub edi, ebx

kernel_row_loop:
	
	movzx eax,  BYTE [edi - 3]     ;first pixel in row
	imul  DWORD [ecx]
	add   ebx,  eax

	movzx eax, BYTE  [edi]
	imul  DWORD [ecx + 4]
	add   ebx,  eax
	
	movzx eax, BYTE  [edi + 3]
	imul  DWORD [ecx + 8]
	add   ebx,  eax
	

	add   ecx, 12
	add   edi, [ebp - 12]
	dec   esi
	jnz kernel_row_loop

	
	
	xor eax, eax
	test  ebx, ebx
	js next

	mov   eax, ebx ; calculate the new value of pixel
	xor   edx, edx
	div   DWORD [ebp - 4]

	mov   esi, 255
	cmp   eax, esi
	cmovg eax, esi

next:
	mov edx,  [ebp + 8]
	
	mov [edx], al

	dec DWORD [ebp + 8]  ;output
	dec DWORD [ebp + 12] ;input 
	dec DWORD [ebp - 8]  ;bytes to process
	jnz filter_loop



epilog:
	pop ebx
	pop edi
	pop esi
	leave
	ret
