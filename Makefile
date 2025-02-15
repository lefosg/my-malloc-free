.PHONY: all clean

# Default target
all: build/main

# Rule to compile main.c and memman.c into an executable
build/main: src/main.c src/memman/memman.c
	gcc -Wall -ggdb src/main.c src/memman/memman.c -o $@

# Clean build files
clean: 
	rm -rf build/*
