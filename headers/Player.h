#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>

class Player {
public:
    Player(SDL_Renderer* renderer, const std::string& path, int screenWidth, int screenHeight, float scale);
    ~Player();
    // these are set on the constructor in the cpp file
    void handleEvent(const SDL_Event& e); // Keyboard input (WASD)
    void update();                        // Update position
    void render();                        // Draw and rotate toward mouse
    void update(float deltaTime);         // used to prevent frame dependent movement

    float getX() const { return x_; }     // used for pathfinding
    float getY() const { return y_; }

private:
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

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
