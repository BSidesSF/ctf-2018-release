CC := gcc
TRIPLET := $(shell $(CC) -dumpmachine)
CFLAGS := -Wall -pie -fPIE -I../common
ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

service: service.o runner.o common.o arm_handoff.o
	$(CC) $(CFLAGS) -o $@ $^ /usr/lib/$(TRIPLET)/libseccomp.a
ifndef DEBUG
	strip $@
	cp $@ ../../distfiles/service
endif

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

common.o: ../common/common.c
	$(CC) $(CFLAGS) -o $@ -c $<

runner.o: runner.S
	nasm -f elf64 -o $@ $<

clean:
	rm service *.o
