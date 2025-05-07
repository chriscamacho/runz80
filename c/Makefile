# Makefile for building your Z80 project

# Project Name
PROJECT_NAME := test1

# Compiler and tools
SDCC := sdcc
SDASZ80 := sdasz80
PYTHON := venv/bin/python # Ensure this points to the Python in your venv

# Compiler flags
CFLAGS := -mz80
CODE_LOC := 0x8000

# Source files
SRC := test1.c
ASM_SRC := crt0.s

# Object files
OBJ := crt0.rel test1.rel

# Output files
IHX_OUTPUT := $(PROJECT_NAME).ihx
BIN_OUTPUT := $(PROJECT_NAME).bin

# Default target
.PHONY: all
all: $(BIN_OUTPUT)

# Rule to create object files from assembly source
crt0.rel: crt0.s
	@echo "Assembling $< to $@..."
	$(SDASZ80) -o $@ $<

# Rule to create object files from C source
test1.rel: test1.c
	@echo "Compiling $< to $@..."
	$(SDCC) $(CFLAGS) -c $< -o $@ --code-loc $(CODE_LOC)

# Rule to link object files and create IHX output
$(IHX_OUTPUT): $(OBJ)
	@echo "Linking object files to create $@"
	$(SDCC) $(CFLAGS) --no-std-crt0 $(OBJ) -o $@ --code-loc $(CODE_LOC)

# Rule to create binary output from IHX
$(BIN_OUTPUT): $(IHX_OUTPUT)
	@echo "Converting $@ from IHX to binary..."
	$(PYTHON) hex2bin.py $< $@

# Clean target to remove intermediate files
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(OBJ) $(IHX_OUTPUT) $(BIN_OUTPUT)
	rm -f $(PROJECT_NAME).asm  $(PROJECT_NAME).lk  $(PROJECT_NAME).lst
	rm -f $(PROJECT_NAME).map  $(PROJECT_NAME).noi  $(PROJECT_NAME).sym
