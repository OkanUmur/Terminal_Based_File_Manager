CC = gcc
CFLAGS = -Wall -g
TARGET = bin/file_manager

SOURCES = src/main.c src/file_operations.c src/directory_ops.c src/permissions.c src/logger.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(TARGET)
