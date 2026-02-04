#include "../headers/SDLManager.h"
#include <iostream>

bool SDLManager::initialized_ = false;

bool SDLManager::initialize() {
    if (initialized_) {
        return true; 
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    initialized_ = true;
    return true;
}

void SDLManager::cleanup() {
    if (!initialized_) {
        return;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    initialized_ = false;
}
