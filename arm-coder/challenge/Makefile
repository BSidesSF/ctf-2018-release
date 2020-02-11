CC := arm-linux-gnueabi-gcc-7  # Not sure why version is needed
CFLAGS := -Wall -pie -fPIE
ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

coder: coder.o runner.o
	$(CC) $(CFLAGS) -o $@ $^ ./libseccomp.a
ifndef DEBUG
	arm-linux-gnueabi-strip $@
	cp $@ ../distfiles/
endif

coder.o: coder.c
	$(CC) $(CFLAGS) -o $@ -c $<

runner.o: runner.S
	arm-linux-gnueabi-as -o $@ $<

clean:
	rm coder *.o
