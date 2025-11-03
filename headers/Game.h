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

/**
 * @class Game
 * @brief Main game manager - handles game loop, collisions, and object updates
 * 
 * This class encapsulates all game logic including:
 * - Object lifecycle management (bullets, asteroids, player)
 * - Collision detection
 * - Game state updates
 * - Rendering coordination
 */
class Game {
public:
    /**
     * @brief Constructor
     * @param renderer SDL renderer for drawing
     * @param screenWidth Screen width in pixels
     * @param screenHeight Screen height in pixels
     * @param sounds Sound manager reference
     */
    Game(SDL_Renderer* renderer, int screenWidth, int screenHeight, Sounds* sounds);
    
    /**
     * @brief Destructor - cleans up all game objects
     */
    ~Game();

    /**
     * @brief Initialize the game (creates player, resets rounds)
     */
    void initialize();

    /**
     * @brief Handle SDL events
     * @param event SDL event to process
     */
    void handleEvent(const SDL_Event& event);

    /**
     * @brief Update all game objects
     * @param deltaTime Time elapsed since last frame in seconds
     * @return true if game should continue, false if game over
     */
    bool update(float deltaTime);

    /**
     * @brief Render all game objects and UI
     */
    void render();

    /**
     * @brief Check if game has started
     * @return true if game is active
     */
    bool isGameStarted() const { return gameStarted_; }

    /**
     * @brief Spawn a new asteroid
     * @param texturePath Path to asteroid texture
     * @param playerX Player X position
     * @param playerY Player Y position
     */
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

    /**
     * @brief Update bullets (movement, collision, cleanup)
     * @param deltaTime Time elapsed since last frame
     */
    void updateBullets(float deltaTime);

    /**
     * @brief Update asteroids (movement, cleanup)
     * @param deltaTime Time elapsed since last frame
     */
    void updateAsteroids(float deltaTime);

    /**
     * @brief Check collisions between bullets and asteroids
     */
    void checkBulletAsteroidCollisions();

    /**
     * @brief Check collisions between player and asteroids
     * @return true if player was hit
     */
    bool checkPlayerAsteroidCollisions();

    /**
     * @brief Clean up all game objects
     */
    void cleanup();

    /**
     * @brief Clear all bullets (used between rounds)
     */
    void clearBullets();
};

#endif // GAME_H
