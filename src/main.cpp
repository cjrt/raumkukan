/**
 * @file main.cpp
 * @brief Main entry point for Raumkūkan game
 * 
 * This file initializes SDL, creates the game window, and runs the main game loop.
 * The actual game logic is handled by the Game class.
 */

#include <SDL2/SDL.h>
#include <iostream>
#include "../headers/SDLManager.h"
#include "../headers/Game.h"
#include "../headers/AnimatedBackground.h"
#include "../headers/StartScreen.h"
#include "../headers/Sound.h"
#include "../headers/GameConstants.h"

/**
 * @brief Main function - entry point of the program
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Exit code (0 for success, non-zero for error)
 */
int main(int argc, char* argv[]) {
    // Initialize SDL subsystems
    if (!SDLManager::initialize()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return 1;
    }

    // Initialize sound system
    Sounds sounds;
    if (!sounds.init()) {
        std::cerr << "Failed to initialize sound system!" << std::endl;
        SDLManager::cleanup();
        return 1;
    }

    // Load audio assets
    if (!sounds.loadMusic(GameConstants::MUSIC_PATH)) {
        SDLManager::cleanup();
        return 1;
    }

    if (!sounds.loadGunSound(GameConstants::GUN_SOUND_PATH)) {
        SDLManager::cleanup();
        return 1;
    }

    sounds.playMusic();

    // Create window
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

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDLManager::cleanup();
        return 1;
    }

    // Create game objects
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

    // Main game loop variables
    Uint32 lastFrameTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    // Main game loop
    while (running) {
        // Calculate delta time
        Uint32 currentFrameTime = SDL_GetTicks();
        float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentFrameTime;

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (!game.isGameStarted()) {
                // Handle start screen
                startScreen.handleEvent(event);
                if (startScreen.shouldStartGame()) {
                    game.initialize();
                }
            } else {
                // Handle game events
                game.handleEvent(event);
            }
        }

        // Update game state
        if (game.isGameStarted()) {
            if (!game.update(deltaTime)) {
                // Game over
                running = false;
            }
        }

        // Update background animation (always running)
        background.update();

        // Render everything
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render background
        background.render();

        // Render game or start screen
        if (!game.isGameStarted()) {
            startScreen.render();
        } else {
            game.render();
        }

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDLManager::cleanup();

    return 0;
}