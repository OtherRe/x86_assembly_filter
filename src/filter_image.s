SECTION .TEXT
	GLOBAL filter_image

filter_image:
	push rbp
	mov rbp, rsp
	push r12
	push r13
	push r14
	push r15

	mov r15, rdx ; saving a pointer to kernel

	;calculating sum of a kernel
	xor r14, r14

	movdqu xmm0, [rdx]
	movdqu xmm1, [rdx + 16]
	paddd  xmm0, xmm1
	movdqa xmm1, xmm0
	psrldq xmm1, 8
	paddd  xmm0, xmm1
	movdqa xmm1, xmm0
	psrldq xmm1, 4
	paddd  xmm0, xmm1
	movd   r14d, xmm0
	add    r14d, [rdx + 32]


	;making sure we are not dividing by zero
	mov rax ,1
	test r14d, r14d
	cmovz r14, rax

	; calculating number of byte we will have to process
	lea r10, [ecx + ecx*2];width in bytes
	lea eax, [r8d - 2]
	mul r10d

	;saving the number into rcx - after mul value is in two registers
	mov ecx, eax
	shl rcx, 32
	shrd rcx, rdx, 32
	

	;rcx ------> whole image to process in bytes
	;r15 ------> pointer to first value in buffer
	;r14 ------> kernel sum

	;r12 ------> pointer to kernel value
	;r11 ------> bytes we need to add to get to correct row
	;r10 ------> whole row in bytes
	;r9  ------> accumulator of suma_wazona
	;r8  ------> temporary holder for pixels
	;rsi ------> input_buffer to process
	;rdi ------> output_buffer to save into

filter_loop:
	xor   r9, r9   ;reset accumulator
	mov   r12, r15 ;reset kernel pointer
	mov   r11, r10 
	neg   r11	   ;reset row value
kernel_row_loop:

	mov   r8,  [rsi + r11 - 3]
	movzx rax, r8b     ;first pixel in row
	imul  DWORD [r12]
	add   r9d,  eax

	shr   r8,  24	   ;second pixel
	movzx rax, r8b
	imul  DWORD [r12 + 4]
	add   r9d,  eax
	
	shr   r8,  24      ;third one
	movzx rax, r8b
	imul   DWORD [r12 + 8]
	add   r9d,  eax

	add   r12, 12
	cmp   r11, r10
	lea   r11, [r11 + r10]
	jne kernel_row_loop


	xor rax, rax
	test  r9d, r9d
	js next

	xor   rdx, rdx; calculate the new value of pixel
	mov rax, r9
	div  r14

	mov   r8, 255
	cmp   eax, r8d
	cmovg eax, r8d

next:
	mov [rdi], al

	dec rdi
	dec rsi
	dec rcx
	jnz filter_loop

epilog:
	pop r15
	pop r14
	pop r13
	pop r12
	mov rsp, rbp
	pop rbp
	ret
