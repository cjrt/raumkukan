#include "../headers/Asteroid.h"
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <cmath>

Asteroid::Asteroid(SDL_Renderer* renderer, const std::string& texturePath,
                   int screenW, int screenH, float playerX, float playerY)
    : renderer_(renderer), texture_(nullptr),
      screenW_(screenW), screenH_(screenH)
{
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (!surface) return;

    texture_ = SDL_CreateTextureFromSurface(renderer, surface);

    // randomize size of asteroid
    float scale = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.3f;

    int w = static_cast<int>(surface->w * scale);
    int h = static_cast<int>(surface->h * scale);
    SDL_FreeSurface(surface);

    // Random rotation speed between -90 and 90 degrees/sec
    rotationSpeed_ = -90.0f + static_cast<float>(rand()) / RAND_MAX * 180.0f;
    angle_ = static_cast<float>(rand() % 360); // random initial angle

    rect_.w = w;
    rect_.h = h;

    // spawn outside the screen randomly
    int side = rand() % 4;
    switch (side) {
        case 0: x_ = rand() % screenW_; y_ = -h; break;           // top
        case 1: x_ = rand() % screenW_; y_ = screenH_; break;     // bottom
        case 2: x_ = -w; y_ = rand() % screenH_; break;           // left
        case 3: x_ = screenW_; y_ = rand() % screenH_; break;     // right
    }

    // give each asteroid a random speed
    speed_ = 100.0f + rand() % 400;

    // direction toward player
    float dx = playerX - x_;
    float dy = playerY - y_;
    float len = std::sqrt(dx * dx + dy * dy);
    vx_ = (dx / len) * speed_;
    vy_ = (dy / len) * speed_;
}

Asteroid::~Asteroid() {
    if (texture_) SDL_DestroyTexture(texture_);
}

void Asteroid::update(float deltaTime, float playerX, float playerY) {

    // update rotation
    angle_ += rotationSpeed_ * deltaTime;

    // keep angle between 0–360
    if (angle_ >= 360.0f) angle_ -= 360.0f;
    else if (angle_ < 0.0f) angle_ += 360.0f;

    // adjust velocity slightly toward player's current position (homing effect)
    float dx = playerX - x_;
    float dy = playerY - y_;
    float len = std::sqrt(dx * dx + dy * dy);
    if (len > 0) {
        vx_ = (dx / len) * speed_;
        vy_ = (dy / len) * speed_;
    }

    x_ += vx_ * deltaTime;
    y_ += vy_ * deltaTime;

    rect_.x = static_cast<int>(x_);
    rect_.y = static_cast<int>(y_);
}

void Asteroid::render() {
    if (texture_) SDL_RenderCopyEx(renderer_, texture_, nullptr, &rect_, angle_, nullptr, SDL_FLIP_NONE);
}

bool Asteroid::isOffScreen() const {
    return x_ < -rect_.w * 2 || x_ > screenW_ + rect_.w * 2 ||
           y_ < -rect_.h * 2 || y_ > screenH_ + rect_.h * 2;
}
