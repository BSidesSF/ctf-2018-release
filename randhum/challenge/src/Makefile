CC?=gcc
LIBS?=
CFLAGS?=-m32
OBJS=randhum.o

all: randhum

randhum: ${OBJS}
	${CC} ${CFLAGS} -o randhum ${OBJS} ${LIBS}

clean:
	rm -fv *.o randhum core core.*
