#include "../headers/Sound.h"
#include <iostream>

Sounds::~Sounds() {
    if (gunSound) Mix_FreeChunk(gunSound);
    if (bgMusic) Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
}

bool Sounds::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

bool Sounds::loadMusic(const std::string& path) {
    bgMusic = Mix_LoadMUS(path.c_str());
    if (!bgMusic) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

bool Sounds::loadGunSound(const std::string& path) {
    gunSound = Mix_LoadWAV(path.c_str());
    if (!gunSound) {
        std::cerr << "Failed to load gun sound: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void Sounds::playMusic() {
    if (bgMusic) Mix_PlayMusic(bgMusic, -1);
}

void Sounds::playGunSound() {
    if (gunSound) Mix_PlayChannel(-1, gunSound, 0);
}
