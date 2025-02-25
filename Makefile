.PHONY: all clean

# Default target
all: build/main build/memman.so

# to run the binary, store both files in same folder or
# with LD_LIBRARY_PATH="path/to/.so/folder;LD_LIBRARY_PATH" ./main
build/main: src/main.c build/memman.so
	gcc -Wall -o $@ $^ -L. 

build/memman.so: src/memman/memman.c src/memman/memman.h
	gcc -Wall -g -fPIC -shared -o $@ src/memman/memman.c -lc

# Clean build files
clean: 
	rm -rf build/*