.section .text
.global execute_alternate
.arm

execute_alternate:
  # r0 = execp
  # r1 = new stack
  MOV sp, r1
  BX r0
