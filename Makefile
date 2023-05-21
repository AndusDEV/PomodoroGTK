CC = g++
CFLAGS = -Wall `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`

TARGET = bin/pomodoro_timer
SRC_DIR = src
SRC = $(SRC_DIR)/pomodoro.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(LIBS) -o $@ $<

clean:
	rm -f $(TARGET)
