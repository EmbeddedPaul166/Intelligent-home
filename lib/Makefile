#Generic makefile for compiling libraries

SHELL := /bin/bash

LIBRARY_SRC_PATH := src
LIBRARY_INC_PATH := include
INC_PATH := ../include
BUILD_PATH := ../build

CC := arm-none-eabi-gcc

CFLAGS += -g -O0 -mthumb -mcpu=cortex-m3 --specs=nosys.specs -Wall -Wextra 

SRC_EXT := c
SOURCES := $(shell find $(LIBRARY_SRC_PATH)/ -type f -name *.$(SRC_EXT))
OBJECTS := $(patsubst $(LIBRARY_SRC_PATH)/%, $(BUILD_PATH)/%, $(SOURCES:.$(SRC_EXT)=.o))

INC := -I $(LIBRARY_INC_PATH) -I $(INC_PATH) 

all: $(OBJECTS)
	
$(BUILD_PATH)/%.o: $(LIBRARY_SRC_PATH)/%.$(SRC_EXT)  
	@echo "Compiling library sources..."
	$(CC) -c $(CFLAGS) $(INC) $< -o $@
	@echo ""
	
	
