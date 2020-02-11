CC := arm-linux-gnueabi-gcc-7  # Not sure why version is needed
CFLAGS := -Wall -pie -fPIE -I ../common
ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

service: service.o runner.o common.o
	$(CC) $(CFLAGS) -o $@ $^ ./libseccomp.a
ifndef DEBUG
	arm-linux-gnueabi-strip $@
	cp $@ ../../distfiles/arm-service
endif

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

common.o: ../common/common.c
	$(CC) $(CFLAGS) -o $@ -c $<

runner.o: runner.S
	arm-linux-gnueabi-as -o $@ $<

clean:
	rm service *.o
