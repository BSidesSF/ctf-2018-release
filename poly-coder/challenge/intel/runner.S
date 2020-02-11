BITS 64

section .text
global execute_alternate
execute_alternate:
  ; RDI = execp
  ; RSI = new stack
  mov rsp, rsi
  jmp rdi
