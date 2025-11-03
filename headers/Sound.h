#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

class Sounds {
public:
    Sounds() = default;
    ~Sounds();

    bool init();
    bool loadMusic(const std::string& path);
    bool loadGunSound(const std::string& path);

    void playMusic();
    void playGunSound();

private:
    Mix_Music* bgMusic = nullptr;
    Mix_Chunk* gunSound = nullptr;
};
