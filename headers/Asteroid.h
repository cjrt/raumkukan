#ifndef ASTEROID_H
#define ASTEROID_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Asteroid {
public:
    Asteroid(SDL_Renderer* renderer, const std::string& texturePath, int screenW, int screenH, float playerX, float playerY);
    ~Asteroid();

    void update(float deltaTime, float playerX, float playerY);
    void render();
    bool isOffScreen() const;

    const SDL_Rect& getRect() const { return rect_; }  // <-- return reference

private:
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    SDL_Rect rect_;
    float x_, y_;
    float vx_, vy_;
    float speed_;
    float angle_, rotationSpeed_;
    int screenW_, screenH_;
};

#endif
