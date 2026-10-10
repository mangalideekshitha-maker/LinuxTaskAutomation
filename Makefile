
CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude
THREAD_FLAGS=-pthread

SRC=src/main.c src/input.c src/parser.c src/process.c src/builtin.c src/signals.c src/pipes.c src/redirect.c src/thread.c src/job_control.c
TARGET=bin/linux_task_automation
DEADLOCK_TARGET=bin/deadlock

all: $(TARGET) $(DEADLOCK_TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(THREAD_FLAGS) -o $(TARGET)

$(DEADLOCK_TARGET): src/deadlock.c
	mkdir -p bin
	$(CC) $(CFLAGS) src/deadlock.c $(THREAD_FLAGS) -o $(DEADLOCK_TARGET)

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

asan:
	mkdir -p bin
	$(CC) $(CFLAGS) -fsanitize=address -fno-omit-frame-pointer $(SRC) $(THREAD_FLAGS) -o $(TARGET)

clean:
	rm -rf bin/*

.PHONY: all run valgrind asan clean
