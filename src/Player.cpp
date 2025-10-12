#include "../headers/Player.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <cmath>
#include <iostream>

Player::Player(SDL_Renderer* renderer, const std::string& path, int screenWidth, int screenHeight, float scale)
    : renderer_(renderer), texture_(nullptr),
      x_(screenWidth / 2.0f), y_(screenHeight / 2.0f),
      speed_(300.0f), scale_(0.175f),
      screenWidth_(screenWidth), screenHeight_(screenHeight),
      movingUp_(false), movingDown_(false), movingLeft_(false), movingRight_(false)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load player image: " << IMG_GetError() << std::endl;
        return;
    }

    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    w_ = static_cast<int>(surface->w * scale_);
    h_ = static_cast<int>(surface->h * scale_);
    SDL_FreeSurface(surface);
}

Player::~Player() {
    if (texture_) SDL_DestroyTexture(texture_);
}

void Player::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && !e.key.repeat) {
        switch (e.key.keysym.sym) {
            case SDLK_w: movingUp_ = true; break;
            case SDLK_s: movingDown_ = true; break;
            case SDLK_a: movingLeft_ = true; break;
            case SDLK_d: movingRight_ = true; break;
        }
    } else if (e.type == SDL_KEYUP && !e.key.repeat) {
        switch (e.key.keysym.sym) {
            case SDLK_w: movingUp_ = false; break;
            case SDLK_s: movingDown_ = false; break;
            case SDLK_a: movingLeft_ = false; break;
            case SDLK_d: movingRight_ = false; break;
        }
    }
}

void Player::update(float deltaTime) {
    float move = speed_ * deltaTime; // move pixels per second

    if (movingUp_)    y_ -= move;
    if (movingDown_)  y_ += move;
    if (movingLeft_)  x_ -= move;
    if (movingRight_) x_ += move;

    int margin = 10;
    if (x_ < margin) x_ = margin;
    if (y_ < margin) y_ = margin;
    if (x_ + w_ > screenWidth_ - margin) x_ = screenWidth_ - w_ - margin;
    if (y_ + h_ > screenHeight_ - margin) y_ = screenHeight_ - h_ - margin;
}

float Player::getAngleToMouse() const {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    float dx = mouseX - (x_ + w_ / 2.0f);
    float dy = mouseY - (y_ + h_ / 2.0f);
    return atan2f(dy, dx) * 180.0f / M_PI;
}

void Player::render() {
    if (!texture_) return;

    SDL_Rect dst { static_cast<int>(x_), static_cast<int>(y_), w_, h_ };
    float angle = getAngleToMouse() + 90.0f;

    SDL_RenderCopyEx(renderer_, texture_, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}
