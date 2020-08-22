CC = gcc
TARGET = main
SRC = main.cc

$(TARGET): $(SRC)
	$(CC) $^ -o $(TARGET)

%.o: %.c
	$(CC) $<
