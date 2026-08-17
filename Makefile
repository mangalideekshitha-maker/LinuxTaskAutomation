CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
SRC = src/main.c \
      src/input.c
TARGET = bin/linux_task_automation

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -rf bin/*
