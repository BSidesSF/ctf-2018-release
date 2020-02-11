BITS 32

# assumes shellcode @eax
mov ecx, 0x24
and eax, eax
add eax, ecx
mov ebx, eax
inc edx
loop:
  mov edx, [eax]
  nop
  add eax, 4
  nop
  mov [ebx], edx
  inc ebx
  inc ebx
  nop
  inc ebx
  nop
  nop
  nop
  dec ecx
  nop
  nop
  nop
  jnz loop
# INT3
nop
