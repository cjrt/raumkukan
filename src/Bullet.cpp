#include "../headers/Bullet.h"
#include <SDL2/SDL_image.h>
#include <cmath>
#include <iostream>

Bullet::Bullet(SDL_Renderer* renderer, const std::string& path, float startX, float startY, float targetX, float targetY)
    : renderer_(renderer), texture_(nullptr), x_(startX), y_(startY), speed_(900.0f) // px/sec
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load bullet image: " << IMG_GetError() << std::endl;
        return;
    }
    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    rect_.w = 50;
    rect_.h = 50;
    SDL_FreeSurface(surface);

    angle = atan2(targetY - startY, targetX - startX) * 180.0f / M_PI - 90.0f; // fix rotation[off by 90 degrees]

    // direction vector normalized * speed
    float dx = targetX - startX;
    float dy = targetY - startY;
    float len = std::sqrt(dx*dx + dy*dy);
    if (len == 0.0f) len = 1.0f;
    vx_ = (dx / len) * speed_;
    vy_ = (dy / len) * speed_;

    rect_.x = static_cast<int>(x_ - rect_.w / 2);
    rect_.y = static_cast<int>(y_ - rect_.h / 2);
}

Bullet::~Bullet() {
    if (texture_) SDL_DestroyTexture(texture_);
}

void Bullet::update(float deltaTime) {
    x_ += vx_ * deltaTime;
    y_ += vy_ * deltaTime;
    rect_.x = static_cast<int>(x_ - rect_.w / 2);
    rect_.y = static_cast<int>(y_ - rect_.h / 2);
}

void Bullet::render() {
    if (!texture_) return;

    // rotate the bullet by its angle so it points in the movement direction
    SDL_Point center = {rect_.w / 2, rect_.h / 2}; // rotate around center
    SDL_RenderCopyEx(renderer_, texture_, nullptr, &rect_, angle + 90.0f, &center, SDL_FLIP_NONE);
}

bool Bullet::isOffScreen() const {
    return x_ < -rect_.w || x_ > 1920 + rect_.w || y_ < -rect_.h || y_ > 1080 + rect_.h;
}
