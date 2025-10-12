#include "../headers/AnimatedBackground.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <sstream>

AnimatedBackground::AnimatedBackground(SDL_Renderer* renderer, const std::string& folder, int frameCount, int frameDelayMs)
    : renderer_(renderer), currentFrame_(0), frameCount_(frameCount), frameDelayMs_(frameDelayMs)
{
    lastUpdate_ = std::chrono::steady_clock::now();

    for (int i = 1; i <= frameCount_; ++i) {
        std::stringstream path;
        path << folder << "/frame" << i << ".png";
        SDL_Surface* surface = IMG_Load(path.str().c_str());
        if (!surface) {
            std::cerr << "Failed to load " << path.str() << ": " << IMG_GetError() << std::endl;
            continue;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_FreeSurface(surface);
        frames_.push_back(tex);
    }
}

// remove from memory if gone[I.E when game is exited]
AnimatedBackground::~AnimatedBackground() {
    for (auto tex : frames_) {
        SDL_DestroyTexture(tex);
    }
}

void AnimatedBackground::update() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate_).count();

    if (elapsed >= frameDelayMs_) {
        currentFrame_ = (currentFrame_ + 1) % frameCount_;
        lastUpdate_ = now;
    }
}

void AnimatedBackground::render() {
    if (frames_.empty()) return;
    SDL_RenderCopy(renderer_, frames_[currentFrame_], nullptr, nullptr);
}
