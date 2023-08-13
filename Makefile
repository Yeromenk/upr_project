BUILD_DIR=build
OBJECTS_DIR=$(BUILD_DIR)/obj

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=$(OBJECTS_DIR)/%.o)

TARGET=photoshop

FLAGS=-fsanitize=address -Wall -g

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $(FLAGS) -o $@ $^

$(OBJECTS): $(SOURCES) $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o: %.c
	gcc $(FLAGS) -c -o $@ $<

$(OBJECTS_DIR):
	mkdir -p $@

clean:
	rm -r $(BUILD_DIR)