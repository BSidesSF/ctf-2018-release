TARGET = level2
LIBS = -lm -lcurses
CC = gcc
CFLAGS = -Wall

.PHONY: default
.PRECIOUS: $(TARGET) $(OBJECTS)
default: $(TARGET)

MAINS=level2.o
OBJECTS = $(filter-out $(MAINS), $(patsubst %.c, %.o, $(wildcard *.c)))
HEADERS = $(wildcard *.h)

level2: level2.o $(OBJECTS)
	$(CC) $(OBJECTS) level2.o -Wall $(LIBS) -o level2

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
