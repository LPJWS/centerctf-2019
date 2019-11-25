global _start

section .text

_start: ; rdi - string ptr
    jmp CODE1+1
    encrypted: dd 0xf0ce25, 0xf06e25, 0xf0be25, 0xf0de25, 0xf31e25, 0xf1de25, 0xf22e25, 0xf7ee25, 0xf3ee25, 0xf15e25, 0xf2be25, 0xf28e25, 0xf7ae25, 0xf3fe25, 0xf3ee25, 0xf15e25, 0xf39e25, 0xf25e25, 0xf27e25, 0xf79e25, 0xf15e25, 0xf2ce25, 0xf7be25, 0xf26e25, 0xf2fe25, 0xf26e25, 0xf2fe25, 0xf39e25, 0xf39e25, 0xf15e25, 0xf3ae25, 0xf2be25, 0xf29e25, 0xf21e25, 0xf2fe25, 0xf38e25, 0xf7fe25, 0xf75e25, 0xf37e25
CODE1:
    DB 0xE9 
    push rbp
    mov rbp, rsp

    call _strlen
    cmp rax, 39
    jne LEAVE_FAIL

    push DWORD 0x0 ; counter

    mov rdi, rdx

LOOP_BODY:
    mov eax, DWORD [rbp-4] ; rbp-4 = counter
    cmp eax, 39 ; rbp-8 = counter
    jl CHECK_CHAR
    jmp LEAVE_CORRECT

CHECK_CHAR:
    xor rcx, rcx
    mov cl, BYTE [rdi]
    sal rcx, 12
    xor rcx, 0xf4ae25
    mov eax, DWORD [rbp-4] ; load counter
    lea rsi, [rel encrypted]; check appropriate block
    imul eax, 4
    add rsi, rax
    cmp ecx, DWORD [rsi]
    jne LEAVE_FAIL

    inc rdi
    inc DWORD [rbp-4] ; (ord(char) << 12) ^ 0xf4ae25
    jmp LOOP_BODY

LEAVE_CORRECT:
    mov rax, 0x1
    leave ; mov rsp, rbp; pop rbp
    ret

LEAVE_FAIL:
    xor rax, rax
    leave ; mov rsp, rbp; pop rbp
    ret

_strlen:
    push rcx            ; save and clear out counter
    xor  rcx, rcx

_strlen_next:
    cmp [rdi], byte 0  ; null byte yet?
    jz  _strlen_null   ; yes, get out

    inc rcx            ; char is ok, count it
    inc rdi            ; move to next char
    jmp _strlen_next   ; process again

_strlen_null:
    mov rax, rcx       ; rcx = the length (put in rax)

    pop rcx            ; restore rcx
    ret                ; get out