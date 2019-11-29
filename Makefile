#Generic makefile for this project

SHELL := /bin/bash

SRC_PATH := src
INC_PATH := include
LIBRARY_SRC_PATH := lib/src
LIBRARY_INC_PATH := lib/include
BUILD_PATH := build
BIN_PATH := bin
SCRIPT_PATH := ld

CC := arm-none-eabi-gcc
OBJ_COPY := arm-none-eabi-objcopy

CFLAGS += -g -O0 -mthumb -mcpu=cortex-m3 --specs=nosys.specs -Wall -Wextra -pedantic

EXECUTABLE := home_management_system
SCRIPT := linker.ld
BIN := $(BIN_PATH)/$(EXECUTABLE)
LD_SCRIPT := $(SCRIPT_PATH)/$(SCRIPT)

SRC_ASM_EXT := S

ASM_SOURCES := $(shell find $(SRC_PATH) -type f -name *.$(SRC_ASM_EXT))
ASM_OBJECTS := $(patsubst $(SRC_PATH)/%, $(BUILD_PATH)/%, $(ASM_SOURCES:.$(SRC_ASM_EXT)=.o))

SRC_C_EXT := c

SOURCES := $(shell find $(SRC_PATH) -type f -name *.$(SRC_C_EXT))
OBJECTS := $(patsubst $(SRC_PATH)/%, $(BUILD_PATH)/%, $(SOURCES:.$(SRC_C_EXT)=.o))

LIB_SOURCES := $(shell find $(LIBRARY_SRC_PATH) -type f -name *.$(SRC_C_EXT))
LIB_OBJECTS := $(patsubst $(LIBRARY_SRC_PATH)/%, $(BUILD_PATH)/%, $(LIB_SOURCES:.$(SRC_C_EXT)=.o))

INC := -I $(INC_PATH) -I $(LIBRARY_INC_PATH) 

$(BIN): $(OBJECTS) $(LIB_OBJECTS) $(ASM_OBJECTS)
	@mkdir -p $(BIN_PATH)	
	@echo "Linking $(BIN)..."
	$(CC) $^ -T $(LD_SCRIPT) $(CFLAGS) -lm -o $(BIN).elf	
	@echo "Linking done"
	@echo ""
	$(OBJ_COPY) -O binary $(BIN_PATH)/$(EXECUTABLE).elf $(BIN_PATH)/$(EXECUTABLE).bin
	@echo "Build done"

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_C_EXT) 
	@mkdir -p $(BUILD_PATH)
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) $(INC) $< -o $@
	@echo ""

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_ASM_EXT)
	@mkdir -p $(BUILD_PATH)
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) $(INC) $< -o $@
	@echo ""

$(BUILD_PATH)/%.o:
	+$(MAKE) -C lib

.PHONY: clean

clean:
	@echo "Cleaning project..."
	rm -r $(BUILD_PATH)/* $(BIN_PATH)/*	
