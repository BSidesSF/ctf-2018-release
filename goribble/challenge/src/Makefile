CFLAGS?=-Wall -fno-stack-protector -m32
LIBS?=-lpthread -lm
all: goribble

clean:
	rm -f goribble *.o core core.*

goribble: goribble.o
	${CC} ${CFLAGS} -o goribble goribble.o ${LIBS}
