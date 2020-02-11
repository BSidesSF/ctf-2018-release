shellcode: shellcode.S
	arm-linux-gnueabi-as -o shellcode shellcode.S
	arm-linux-gnueabi-objcopy -I elf32-little -O binary ./shellcode
