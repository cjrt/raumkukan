#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include <string>

class Bullet {
public:
    Bullet(SDL_Renderer* renderer, const std::string& path, float startX, float startY, float targetX, float targetY);
    ~Bullet();

    void update(float deltaTime);
    void render();
    bool isOffScreen() const;

    const SDL_Rect& getRect() const { return rect_; }

private:
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    SDL_Rect rect_;
    float x_, y_;
    float vx_, vy_;
    float speed_;
    float angle;
};

#endif
