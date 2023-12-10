# Compiler
CC = g++

# Compilation parameters
CompileParms = -g -c -Wall -std=c++11 -O2

# Object files
OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))

# Default target
all: main
build: main

# Target to run the executable
run: main
	./main positives.txt

neg: main
	./main negatives.txt

# Target to link the object files and create the main executable
main: $(OBJS)
	$(CC) -o main $(OBJS)

# Compilation rules
main.o: main.cc parser.h
	$(CC) $(CompileParms) main.cc

parser.o: parser.cc parser.h
	$(CC) $(CompileParms) parser.cc

interpreter.o: interpreter.cc interpreter.h
	$(CC) $(CompileParms) interpreter.cc

# Target to clean the build directory
clean:
	rm -f *.o main
