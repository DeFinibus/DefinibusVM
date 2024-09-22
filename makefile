# Variables
CC = gcc
CFLAGS = -Wall -g
TARGET = zvm
SRC_DIR = kernel
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Targets
all: $(TARGET)

$(TARGET): $(OBJ_DIR)/vm_core.o $(OBJ_DIR)/logging.o $(OBJ_DIR)/zvm.o  $(OBJ_DIR)/vm_bios.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/vm_core.o: $(SRC_DIR)/vm_core.c
	$(CC) $(CFLAGS) -c $< -o $@


$(OBJ_DIR)/logging.o: $(SRC_DIR)/logging.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/vm_bios.o: $(SRC_DIR)/vm_bios.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/zvm.o: zvm.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*

