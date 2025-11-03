#ifndef SDL_MANAGER_H
#define SDL_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/**
 * @class SDLManager
 * @brief Handles SDL initialization and cleanup
 * 
 * RAII wrapper for SDL subsystems to ensure proper initialization
 * and cleanup order.
 */
class SDLManager {
public:
    /**
     * @brief Initialize SDL subsystems
     * @return true if successful, false otherwise
     */
    static bool initialize();

    /**
     * @brief Cleanup all SDL subsystems
     */
    static void cleanup();

private:
    static bool initialized_;
};

#endif // SDL_MANAGER_H
