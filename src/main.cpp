#include <SDL2/SDL.h>
#include <iostream>
#include "../headers/SDLManager.h"
#include "../headers/Game.h"
#include "../headers/AnimatedBackground.h"
#include "../headers/StartScreen.h"
#include "../headers/Sound.h"
#include "../headers/GameConstants.h"

int main(int argc, char* argv[]) {
    if (!SDLManager::initialize()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return 1;
    }

    Sounds sounds;
    if (!sounds.init()) {
        std::cerr << "Failed to initialize sound system!" << std::endl;
        SDLManager::cleanup();
        return 1;
    }

    if (!sounds.loadMusic(GameConstants::MUSIC_PATH)) {
        SDLManager::cleanup();
        return 1;
    }

    if (!sounds.loadGunSound(GameConstants::GUN_SOUND_PATH)) {
        SDLManager::cleanup();
        return 1;
    }

    sounds.playMusic();

    SDL_Window* window = SDL_CreateWindow("Raumkūkan",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          GameConstants::SCREEN_WIDTH,
                                          GameConstants::SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDLManager::cleanup();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDLManager::cleanup();
        return 1;
    }

    AnimatedBackground background(renderer, 
                                  GameConstants::BACKGROUND_FOLDER_PATH,
                                  GameConstants::BACKGROUND_FRAME_COUNT,
                                  GameConstants::BACKGROUND_FRAME_DELAY_MS);
    
    StartScreen startScreen(renderer, 
                           GameConstants::SCREEN_WIDTH, 
                           GameConstants::SCREEN_HEIGHT);
    
    Game game(renderer, 
              GameConstants::SCREEN_WIDTH,
              GameConstants::SCREEN_HEIGHT,
              &sounds);

    Uint32 lastFrameTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    // game loop
    while (running) {
        // delta time
        Uint32 currentFrameTime = SDL_GetTicks();
        float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentFrameTime;

        // events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (!game.isGameStarted()) {
                startScreen.handleEvent(event);
                if (startScreen.shouldStartGame()) {
                    game.initialize();
                }
            } else {
                game.handleEvent(event);
            }
        }

        // game state
        if (game.isGameStarted()) {
            if (!game.update(deltaTime)) {
                // game over
                running = false;
            }
        }

        background.update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        background.render();

        if (!game.isGameStarted()) {
            startScreen.render();
        } else {
            game.render();
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDLManager::cleanup();

    return 0;
}