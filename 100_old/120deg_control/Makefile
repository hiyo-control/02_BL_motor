
COMPILER = gcc
CFLAGS = -Wall
LDFLAGS = -lm

BUILD_DIR = build

.PHONY: all clean

all: $(BUILD_DIR) $(BUILD_DIR)/bldc6p 

$(BUILD_DIR)/bldc6p: $(BUILD_DIR)/bldc6p.o
	$(COMPILER) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lpigpio -lpthread

$(BUILD_DIR)/bldc6p.o: bldc6p.c
	$(COMPILER) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean: $(BUILD_DIR)
	rm build/*