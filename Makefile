CXX = g++
CXXFLAGS = -Wall -std=c++17 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image

TARGET = raumkūkan
BIN = bin
SRCS = src/main.cpp src/AnimatedBackground.cpp src/Player.cpp src/Asteroid.cpp
OBJS = $(SRCS:.cpp=.o)
OUT = $(BIN)/$(TARGET)

all: $(BIN) $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS) $(LDFLAGS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
	rm -rf $(BIN)

.PHONY: all run clean
