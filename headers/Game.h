#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Player;
class Asteroid;
class Bullet;
class AnimatedBackground;
class RoundManager;
class UIRenderer;
class Sounds;

class Game {
public:
 
    Game(SDL_Renderer* renderer, int screenWidth, int screenHeight, Sounds* sounds);
    ~Game();
    void initialize();
    void handleEvent(const SDL_Event& event);

    bool update(float deltaTime);
    void render();
    bool isGameStarted() const { return gameStarted_; }
    void spawnAsteroid(const std::string& texturePath, float playerX, float playerY);

private:
    SDL_Renderer* renderer_;
    int screenWidth_;
    int screenHeight_;
    Sounds* sounds_;
    
    Player* player_;
    RoundManager* roundManager_;
    UIRenderer* uiRenderer_;
    
    std::vector<Bullet*> bullets_;
    std::vector<Asteroid*> asteroids_;
    
    bool gameStarted_;
    bool gameOver_;

    void updateBullets(float deltaTime);
    void updateAsteroids(float deltaTime);
    void checkBulletAsteroidCollisions();
    bool checkPlayerAsteroidCollisions();
    void cleanup();
    void clearBullets();
};

#endif 
