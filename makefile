CC = g++
CFLAGS = -g -Wall -std=c++11
LDFLAGS = -lSDL2

SOURCES = main.cpp CPU.cpp
OBJECTS = $(SOURCES:.cpp = .o)
TARGET = 8080emu

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
.PHONY: clean

clean:
	@rm $(TARGET)
