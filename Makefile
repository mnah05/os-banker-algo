# Banker's Algorithm Makefile
# Deadlock Avoidance Simulation in Cloud Computing

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = banker
SOURCES = main.c banker.c
HEADERS = banker.h

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
