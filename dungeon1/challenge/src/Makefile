TARGET = level1
LIBS = -lm -lcurses
CC = gcc
CFLAGS = -Wall -s

.PHONY: default
.PRECIOUS: $(TARGET) $(OBJECTS)
default: $(TARGET)

MAINS=level1.o level2.o level3.o
OBJECTS = $(filter-out $(MAINS), $(patsubst %.c, %.o, $(wildcard *.c)))
HEADERS = $(wildcard *.h)

level1: level1.o $(OBJECTS)
	$(CC) $(OBJECTS) level1.o -Wall $(LIBS) -o level1


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
