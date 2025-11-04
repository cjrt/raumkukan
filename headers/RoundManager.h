#ifndef ROUND_MANAGER_H
#define ROUND_MANAGER_H

#include <vector>

class Asteroid;

class RoundManager {
public:
    RoundManager();
    
    void resetToRoundOne();
    void updateTransitionTimer(float deltaTime);
    bool shouldSpawnAsteroids(float deltaTime);

    // flag asteroids as spawned, with the number of asteroids spawned
    void markAsteroidsSpawned(int count);

    bool isRoundComplete(size_t activeAsteroidCount) const;
    void advanceToNextRound();
    int getCurrentRound() const { return currentRound_; }
    bool isShowingTransition() const { return showingRoundTransition_; }
    int getAsteroidsToSpawn() const { return asteroidsToSpawnThisRound_; }

    // return time between spawn events(seconds)
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

    int calculateAsteroidsForRound(int round) const;
    float calculateSpawnIntervalForRound(int round) const;
};

#endif 
