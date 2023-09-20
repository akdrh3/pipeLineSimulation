# Compiler and compiler flags
CC = gcc
CFLAGS = -g -Wall -Wextra

# Source and object files
SRC = pipeLine.c
OBJ = $(SRC:.c=.o)

# Executable name
EXECUTABLE = pipeline

# Input and output files
INPUT_FILE = test.in
OUTPUT_FILE = out.txt

# Default target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program with the input file and save output to OUTPUT_FILE
test: $(EXECUTABLE)
	./$(EXECUTABLE) $(INPUT_FILE) > $(OUTPUT_FILE)

# Clean rule
clean:
	rm -f $(OBJ) $(EXECUTABLE) $(OUTPUT_FILE)

.PHONY: all run clean
