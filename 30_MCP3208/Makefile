COMPILER = g++
CFLAGS = -Wall
LDFLAGS = -lm -lwiringPi -lpigpio -lpthread

BUILD_DIR = build

HEADER  = ./include/MCP3208.h
HEADER += ./include/MCP3208.cpp

.PHONY: all clean

all: $(BUILD_DIR) $(BUILD_DIR)/main

$(BUILD_DIR)/main: $(BUILD_DIR)/main.o
	$(COMPILER) $(CFLAGS) -o $@ $^ $(HEADER) $(LDFLAGS)

$(BUILD_DIR)/main.o: main.cpp
	$(COMPILER) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean: $(BUILD_DIR)
	rm build/*