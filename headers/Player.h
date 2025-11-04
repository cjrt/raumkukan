#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Bullet;
class Sounds;

class Player {
public:
    Player(SDL_Renderer* renderer, const std::string& path, int screenWidth, int screenHeight, float scale, Sounds* sounds = nullptr);
    ~Player();
    // these are set on the constructor in the cpp file
    void handleEvent(const SDL_Event& e); // Keyboard input (WASD)
    void render();                       
    void update(float deltaTime);      

    SDL_Rect getHitbox() const;

    float getX() const { return x_; }     // used for pathfinding
    float getY() const { return y_; }

    void shoot(std::vector<Bullet*>& bullets); // fire a bullet, puts a new bullet into the bullets vector

private:
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    Sounds* sounds_ = nullptr;
    float x_, y_;         // Position
    int w_, h_;           // Sprite size
    float speed_;         // Movement speed
    float scale_;         // How big/small u want the sprite to be cuh
    int screenWidth_;
    int screenHeight_;

    // Movement flags
    bool movingUp_;
    bool movingDown_;
    bool movingLeft_;
    bool movingRight_;

    float getAngleToMouse() const; // Helper function to rotate towards the mouse

};

#endif
