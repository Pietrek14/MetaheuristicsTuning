SRC_FILES := main.cpp
DEST_DIR := out
DEST_FILENAME := main.exe
CC := g++
STD := c++23

build:
	if not exist $(DEST_DIR) mkdir $(DEST_DIR)
	$(CC) $(SRC_FILES) -o ./$(DEST_DIR)/$(DEST_FILENAME) -std=$(STD)

run:
	./$(DEST_DIR)/$(DEST_FILENAME)
