#ifndef ROUND_MANAGER_H
#define ROUND_MANAGER_H

#include <vector>

class Asteroid;

/**
 * @class RoundManager
 * @brief Manages round-based gameplay logic
 * 
 * Handles round progression, asteroid spawning quotas, spawn timing,
 * and round completion detection.
 */
class RoundManager {
public:
    RoundManager();
    
    /**
     * @brief Reset to round 1 (called when game starts)
     */
    void resetToRoundOne();

    /**
     * @brief Update round transition timer
     * @param deltaTime Time elapsed since last frame
     */
    void updateTransitionTimer(float deltaTime);

    /**
     * @brief Check if it's time to spawn asteroids
     * @param deltaTime Time elapsed since last frame
     * @return true if asteroids should be spawned
     */
    bool shouldSpawnAsteroids(float deltaTime);

    /**
     * @brief Mark that asteroids have been spawned
     * @param count Number of asteroids spawned
     */
    void markAsteroidsSpawned(int count);

    /**
     * @brief Check if round is complete
     * @param activeAsteroidCount Number of asteroids currently active
     * @return true if round should advance
     */
    bool isRoundComplete(size_t activeAsteroidCount) const;

    /**
     * @brief Advance to the next round
     */
    void advanceToNextRound();

    /**
     * @brief Get current round number
     * @return Current round number
     */
    int getCurrentRound() const { return currentRound_; }

    /**
     * @brief Check if round transition is being shown
     * @return true if showing transition
     */
    bool isShowingTransition() const { return showingRoundTransition_; }

    /**
     * @brief Get number of asteroids that should spawn this round
     * @return Asteroid spawn quota for current round
     */
    int getAsteroidsToSpawn() const { return asteroidsToSpawnThisRound_; }

    /**
     * @brief Get current spawn interval
     * @return Time between spawn events in seconds
     */
    float getSpawnInterval() const { return spawnInterval_; }

private:
    int currentRound_;
    int asteroidsToSpawnThisRound_;
    int asteroidsSpawnedThisRound_;
    float spawnTimer_;
    float spawnInterval_;
    bool showingRoundTransition_;
    float roundTransitionTimer_;
    
    static constexpr float ROUND_TRANSITION_DURATION = 2.0f;
    static constexpr int INITIAL_ASTEROIDS = 10;
    static constexpr int ASTEROIDS_INCREMENT = 5;
    static constexpr float INITIAL_SPAWN_INTERVAL = 0.75f;
    static constexpr float MIN_SPAWN_INTERVAL = 0.3f;
    static constexpr float SPAWN_INTERVAL_DECREASE = 0.05f;

    /**
     * @brief Calculate number of asteroids for a given round
     */
    int calculateAsteroidsForRound(int round) const;

    /**
     * @brief Calculate spawn interval for a given round
     */
    float calculateSpawnIntervalForRound(int round) const;
};

#endif // ROUND_MANAGER_H
