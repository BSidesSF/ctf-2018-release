CC=gcc
CFLAGS?=-Wall
LIBS?=-lgmp
all: signit

clean:
	rm -f signit *.o core core.*

signit: signit.o crypto.o
	${CC} ${CFLAGS} -o signit signit.o crypto.o ${LIBS}
