TARGET = level3
LIBS = -lm -lcurses
CC = gcc
CFLAGS = -Wall

.PHONY: default
.PRECIOUS: $(TARGET) $(OBJECTS)
default: $(TARGET)

MAINS=level3.o
OBJECTS = $(filter-out $(MAINS), $(patsubst %.c, %.o, $(wildcard *.c)))
HEADERS = $(wildcard *.h)

level3: level3.o $(OBJECTS)
	$(CC) $(OBJECTS) level3.o -Wall $(LIBS) -o level3

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
