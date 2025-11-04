#ifndef ANIMATED_BACKGROUND_H
#define ANIMATED_BACKGROUND_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <chrono>

class AnimatedBackground {
public:
    AnimatedBackground(SDL_Renderer* renderer, const std::string& folder, int frameCount, int frameDelayMs);
    ~AnimatedBackground();

    void update();
    void render();

private:
    SDL_Renderer* renderer_;
    std::vector<SDL_Texture*> frames_;
    int currentFrame_;
    int frameCount_;
    int frameDelayMs_;
    std::chrono::steady_clock::time_point lastUpdate_;
};

#endif
