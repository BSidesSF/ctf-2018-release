BITS 32

section .text
global execute_alternate
execute_alternate:
  pop edx
  pop eax
  pop esp
  jmp eax
