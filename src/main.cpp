#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../headers/AnimatedBackground.h"
#include "../headers/Player.h"
#include "../headers/Asteroid.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL/IMG could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Raumkūkan",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    // general objects constructors, background, player etc
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    AnimatedBackground bg(renderer, "assets/background", 9, 100); // 9 frames, change every 5 loops
    Player player(renderer, "assets/player.png", SCREEN_WIDTH, SCREEN_HEIGHT, 0.25f);

    // keep objects relative to frames, prevents frame dependent movement
    Uint32 lastTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    // spawn seed for asteroids
    srand(static_cast<unsigned>(time(nullptr)));
    std::vector<Asteroid*> asteroids;
    float spawnTimer = 0.0f;
    float spawnInterval = 0.75f; // seconds between spawns

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            player.handleEvent(event);
        }

        spawnTimer += deltaTime;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;
            asteroids.push_back(new Asteroid(renderer, "assets/asteroid.png",
                                             SCREEN_WIDTH, SCREEN_HEIGHT,
                                             player.getX(), player.getY()));
        }

        // --- UPDATE ---
        bg.update();
        player.update(deltaTime);

        for (auto it = asteroids.begin(); it != asteroids.end();) {
            (*it)->update(deltaTime, player.getX(), player.getY());
            if ((*it)->isOffScreen()) {
                delete *it;
                it = asteroids.erase(it);
            } else {
                ++it;
            }
        }

        // --- RENDER ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        bg.render();
        for (auto* a : asteroids)
            a->render();
        player.render();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
