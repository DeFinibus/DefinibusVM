# Variables
CC = gcc
CFLAGS = -Wall -g
TARGET = dfbvm
SRC_DIR = kernel
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Targets
all: $(TARGET)

$(TARGET): $(OBJ_DIR)/vm_core.o $(OBJ_DIR)/logging.o $(OBJ_DIR)/dfbvm.o  $(OBJ_DIR)/vm_bios.o  $(OBJ_DIR)/dissasembler.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/vm_core.o: $(SRC_DIR)/vm_core.c
	$(CC) $(CFLAGS) -c $< -o $@


$(OBJ_DIR)/logging.o: $(SRC_DIR)/logging.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/vm_bios.o: $(SRC_DIR)/vm_bios.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/dfbvm.o: dfbvm.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/dissasembler.o: disassembler.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*
	rm -f $(BIN_DIR)/*

