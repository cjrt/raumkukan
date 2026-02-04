CXX = g++
CXXFLAGS = -Wall -std=c++17 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf

TARGET = raumkūkan
BIN = bin
SRCS = src/main.cpp \
       src/AnimatedBackground.cpp \
       src/Player.cpp \
       src/Asteroid.cpp \
       src/Bullet.cpp \
       src/Sound.cpp \
       src/StartScreen.cpp \
       src/Game.cpp \
       src/RoundManager.cpp \
       src/UIRenderer.cpp \
       src/SDLManager.cpp
OBJS = $(SRCS:.cpp=.o)
OUT = $(BIN)/$(TARGET)

all: $(BIN) $(OUT)

$(BIN):
	mkdir -p $(BIN)

$(OUT): $(SRCS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS) $(LDFLAGS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
	rm -rf $(BIN)

.PHONY: all run clean
