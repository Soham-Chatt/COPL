# Compiler
CC = g++

# Compilation parameters
CompileParms = -c -g -Wall -std=c++14 -O2

# Object files
OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))

# Default target
all: main

# Target for executable
run: main
	./main

pos: main
	./main positive.txt

neg: main
	./main negative.txt

# Target link objects
main: $(OBJS)
	$(CC) -o main $(OBJS)

# Rule for object files
%.o: %.cc
	$(CC) $(CompileParms) -I. -c $< -o $@

# Target for clean
clean:
	rm -f *.o main
