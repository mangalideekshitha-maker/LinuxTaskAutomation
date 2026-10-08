CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude

SRC=src/main.c src/input.c src/parser.c src/process.c src/builtin.c src/signals.c src/pipes.c src/redirect.c src/thread.c
TARGET=bin/linux_task_automation

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -pthread -o $(TARGET)
run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

asan:
	mkdir -p bin
	$(CC) $(CFLAGS) -fsanitize=address -fno-omit-frame-pointer $(SRC) -pthread -o $(TARGET)

clean:
	rm -rf bin/*
