#Generic makefile for this project

SHELL := /bin/bash

SRC_PATH := src
BUILD_PATH := build
BIN_PATH := bin
SCRIPT_PATH := scripts

CC := arm-none-eabi-gcc
LD := arm-none-eabi-ld
CFLAGS += -g -O0 -mthumb -Wall -Wextra -pedantic

EXECUTABLE := home_management_system
SCRIPT := linker.ld
BIN := $(BIN_PATH)/$(EXECUTABLE)
LD_SCRIPT := $(SCRIPT_PATH)/$(SCRIPT)

SRC_EXT := c
SOURCES := $(shell find $(SRC_PATH) -type f -name *.$(SRC_EXT))
OBJECTS := $(patsubst $(SRC_PATH)/%, $(BUILD_PATH)/%, $(SOURCES:.$(SRC_EXT)=.o))

INC_PATH := $(shell find include/ -name '*.h' -exec dirname {} \; | sort | uniq)
INC_LIST := $(patsubst include/%, -I include/%, $(INC_PATH))
BUILD_LIST := $(patsubst include/%, $(BUILD_PATH)/%, $(INC_PATH))

INC := -I $(INC_LIST)

LIB := -L /usr/local/lib

$(BIN): $(OBJECTS)
	@mkdir -p $(BIN_PATH)
	@echo "Linking $(BIN)..."
ifdef LIB
	$(LD) $^ -T $(LD_SCRIPT) -o $(BIN) $(LIB)
else
	$(LD) $^ -T $(LD_SCRIPT) -o $(BIN)
endif
	@echo "Linking done"

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@mkdir -p $(BUILD_PATH)
	@echo "Compiling $<..."
ifdef INC_LIST
	$(CC) -c $(CFLAGS) $(INC) -o $@ $<
else
	$(CC) -c $(CFLAGS) -o $@ $<
endif

.PHONY: clean

clean:
	@echo "Cleaning project..."
	rm -r $(BUILD_PATH)/* $(BIN_PATH)/*	
