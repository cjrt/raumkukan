#ifndef SDL_MANAGER_H
#define SDL_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// RAII(Resource Acquisition Is Initialization I never remeber that acronym)

class SDLManager {
public:

    static bool initialize();
    static void cleanup();

private:
    static bool initialized_; // yes? no? maybe? hotel? trivago
};

#endif 
