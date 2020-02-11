CC := gcc
TRIPLET := $(shell $(CC) -dumpmachine)
CFLAGS := -Wall -pie -fPIE
ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

coder: coder.o runner.o
	$(CC) $(CFLAGS) -o $@ $^ /usr/lib/$(TRIPLET)/libseccomp.a
ifndef DEBUG
	strip $@
	cp $@ ../distfiles/
endif

coder.o: coder.c
	$(CC) $(CFLAGS) -o $@ -c $<

runner.o: runner.S
	nasm -f elf64 -o $@ $<

clean:
	rm coder *.o
