CC := gcc
TRIPLET := i386-linux-gnu
CFLAGS := -Wall -pie -fPIE -m32
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
	nasm -f elf32 -o $@ $<

clean:
	rm coder *.o
