CC = g++
CFLAGS = -g -Wall -std=c++11
LDFLAGS = -lSDL2 -lSDL2_mixer -lpthread

SOURCES = main.cpp CPU.cpp Game.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = invaders

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
.PHONY: clean

clean:
	@rm $(TARGET)
