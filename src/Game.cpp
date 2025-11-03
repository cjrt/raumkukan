#include "../headers/Game.h"
#include "../headers/Player.h"
#include "../headers/Asteroid.h"
#include "../headers/Bullet.h"
#include "../headers/RoundManager.h"
#include "../headers/UIRenderer.h"
#include "../headers/GameConstants.h"
#include "../headers/Sound.h"
#include <algorithm>

Game::Game(SDL_Renderer* renderer, int screenWidth, int screenHeight, Sounds* sounds)
    : renderer_(renderer),
      screenWidth_(screenWidth),
      screenHeight_(screenHeight),
      sounds_(sounds),
      player_(nullptr),
      roundManager_(nullptr),
      uiRenderer_(nullptr),
      gameStarted_(false),
      gameOver_(false) {
    
    // Initialize managers
    roundManager_ = new RoundManager();
    uiRenderer_ = new UIRenderer(renderer, GameConstants::FONT_PATH,
                                 GameConstants::ROUND_DISPLAY_FONT_SIZE,
                                 GameConstants::ROUND_TRANSITION_FONT_SIZE);
}

Game::~Game() {
    cleanup();
    if (roundManager_) delete roundManager_;
    if (uiRenderer_) delete uiRenderer_;
}

void Game::initialize() {
    // Create player
    if (!player_) {
        player_ = new Player(renderer_, GameConstants::PLAYER_TEXTURE_PATH,
                           screenWidth_, screenHeight_,
                           GameConstants::PLAYER_SCALE, sounds_);
    }
    
    // Reset round system
    roundManager_->resetToRoundOne();
    gameStarted_ = true;
    gameOver_ = false;
    
    // Clear any existing objects
    clearBullets();
    for (auto* asteroid : asteroids_) delete asteroid;
    asteroids_.clear();
}

void Game::handleEvent(const SDL_Event& event) {
    if (!gameStarted_ || gameOver_) return;

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        player_->shoot(bullets_);
    }
    
    player_->handleEvent(event);
}

bool Game::update(float deltaTime) {
    if (!gameStarted_ || gameOver_) return !gameOver_;

    // Update round transition timer
    roundManager_->updateTransitionTimer(deltaTime);

    // Spawn asteroids based on round manager
    if (roundManager_->shouldSpawnAsteroids(deltaTime)) {
        spawnAsteroid(GameConstants::ASTEROID_TEXTURE_PATH,
                     player_->getX(), player_->getY());
        spawnAsteroid(GameConstants::ASTEROID2_TEXTURE_PATH,
                     player_->getX(), player_->getY());
        roundManager_->markAsteroidsSpawned(GameConstants::ASTEROIDS_PER_SPAWN_EVENT);
    }

    // Check if round is complete
    if (roundManager_->isRoundComplete(asteroids_.size())) {
        roundManager_->advanceToNextRound();
        clearBullets();
    }

    // Update player
    player_->update(deltaTime);

    // Update game objects
    updateBullets(deltaTime);
    updateAsteroids(deltaTime);

    // Check collisions
    checkBulletAsteroidCollisions();
    
    if (checkPlayerAsteroidCollisions()) {
        gameOver_ = true;
        return false;
    }

    return true;
}

void Game::render() {
    if (!gameStarted_) return;

    // Render game objects
    for (auto* asteroid : asteroids_) {
        asteroid->render();
    }
    
    for (auto* bullet : bullets_) {
        bullet->render();
    }
    
    player_->render();

    // Render UI
    if (uiRenderer_ && uiRenderer_->isInitialized()) {
        // Round number display (top left)
        std::string roundText = "Round " + std::to_string(roundManager_->getCurrentRound());
        SDL_Color white = {255, 255, 255, 255};
        uiRenderer_->renderText(roundText,
                                GameConstants::ROUND_TEXT_X_OFFSET,
                                GameConstants::ROUND_TEXT_Y_OFFSET,
                                0, white);

        // Round transition message (centered)
        if (roundManager_->isShowingTransition()) {
            std::string transitionText = "Round " + std::to_string(roundManager_->getCurrentRound());
            SDL_Color yellow = {255, 255, 0, 255};
            uiRenderer_->renderTextCentered(transitionText,
                                          screenWidth_ / 2,
                                          screenHeight_ / 2,
                                          1, yellow);
        }
    }
}

void Game::spawnAsteroid(const std::string& texturePath, float playerX, float playerY) {
    asteroids_.push_back(new Asteroid(renderer_, texturePath,
                                     screenWidth_, screenHeight_,
                                     playerX, playerY));
}

void Game::updateBullets(float deltaTime) {
    for (auto it = bullets_.begin(); it != bullets_.end();) {
        (*it)->update(deltaTime);
        
        if ((*it)->isOffScreen()) {
            delete *it;
            it = bullets_.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::updateAsteroids(float deltaTime) {
    for (auto it = asteroids_.begin(); it != asteroids_.end();) {
        (*it)->update(deltaTime, player_->getX(), player_->getY());
        
        if ((*it)->isOffScreen()) {
            delete *it;
            it = asteroids_.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::checkBulletAsteroidCollisions() {
    for (auto bulletIt = bullets_.begin(); bulletIt != bullets_.end();) {
        bool hit = false;
        SDL_Rect bulletRect = (*bulletIt)->getRect();

        for (auto asteroidIt = asteroids_.begin(); asteroidIt != asteroids_.end(); ++asteroidIt) {
            if (SDL_HasIntersection(&bulletRect, &(*asteroidIt)->getRect())) {
                // Bullet hit asteroid - destroy both
                delete *asteroidIt;
                asteroids_.erase(asteroidIt);
                hit = true;
                break; // Bullet can only hit one asteroid
            }
        }

        if (hit) {
            delete *bulletIt;
            bulletIt = bullets_.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }
}

bool Game::checkPlayerAsteroidCollisions() {
    SDL_Rect playerHitbox = player_->getHitbox();
    
    for (auto* asteroid : asteroids_) {
        if (SDL_HasIntersection(&playerHitbox, &asteroid->getRect())) {
            return true; // Player hit!
        }
    }
    
    return false;
}

void Game::cleanup() {
    clearBullets();
    
    for (auto* asteroid : asteroids_) {
        delete asteroid;
    }
    asteroids_.clear();
    
    if (player_) {
        delete player_;
        player_ = nullptr;
    }
}

void Game::clearBullets() {
    for (auto* bullet : bullets_) {
        delete bullet;
    }
    bullets_.clear();
}
