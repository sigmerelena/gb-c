CC=gcc
CFLAGS=-Wall
SOURCES=src/main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=build/main

all: $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	mkdir -p build
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
	rm -f $(OBJECTS) $(EXECUTABLE)
