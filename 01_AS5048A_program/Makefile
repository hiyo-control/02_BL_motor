COMPILER = gcc
CFLAGS = -Wall
LDFLAGS = -lm

BUILD_DIR = build

HEADER = ./include/AS5048A.h

.PHONY: all clean

all: $(BUILD_DIR) $(BUILD_DIR)/main

$(BUILD_DIR)/main: $(BUILD_DIR)/main.o
	$(COMPILER) $(CFLAGS) -o $@ $^ $(HEADER) $(LDFLAGS)

$(BUILD_DIR)/main.o: main.c
	$(COMPILER) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean: $(BUILD_DIR)
	rm build/*