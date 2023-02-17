# make: compile the program
# make run: run the program
# make clean: remove the build directory

SOURCE_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

SOURCES := $(shell find $(SOURCE_DIR) -name *.c)
OBJECTS := $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/prog: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ -lcurses -lm

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -g -c $< -o $@ -I$(INCLUDE_DIR)

.PHONY: run
run: $(BUILD_DIR)/prog
	./$<

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
