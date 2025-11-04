#include "../headers/RoundManager.h"
#include <algorithm>

RoundManager::RoundManager()
    : currentRound_(1),
      asteroidsToSpawnThisRound_(INITIAL_ASTEROIDS),
      asteroidsSpawnedThisRound_(0),
      spawnTimer_(0.0f),
      spawnInterval_(INITIAL_SPAWN_INTERVAL),
      showingRoundTransition_(false),
      roundTransitionTimer_(0.0f) {
}

void RoundManager::resetToRoundOne() {
    currentRound_ = 1;
    asteroidsToSpawnThisRound_ = INITIAL_ASTEROIDS;
    asteroidsSpawnedThisRound_ = 0;
    spawnInterval_ = INITIAL_SPAWN_INTERVAL;
    showingRoundTransition_ = true;
    roundTransitionTimer_ = 0.0f;
}

void RoundManager::updateTransitionTimer(float deltaTime) {
    if (showingRoundTransition_) {
        roundTransitionTimer_ += deltaTime;
        if (roundTransitionTimer_ >= ROUND_TRANSITION_DURATION) {
            showingRoundTransition_ = false;
            roundTransitionTimer_ = 0.0f;
        }
    }
}

bool RoundManager::shouldSpawnAsteroids(float deltaTime) {
    if (showingRoundTransition_ || 
        asteroidsSpawnedThisRound_ >= asteroidsToSpawnThisRound_) {
        return false;
    }

    spawnTimer_ += deltaTime;
    if (spawnTimer_ >= spawnInterval_) {
        spawnTimer_ = 0.0f;
        return true;
    }

    return false;
}

void RoundManager::markAsteroidsSpawned(int count) {
    asteroidsSpawnedThisRound_ += count;
}

bool RoundManager::isRoundComplete(size_t activeAsteroidCount) const {
    // rounds complete if
    // not showing transition and number of asteroids per round has been spawned and destroyed
    return !showingRoundTransition_ &&
           asteroidsSpawnedThisRound_ >= asteroidsToSpawnThisRound_ &&
           activeAsteroidCount == 0;
}

void RoundManager::advanceToNextRound() {
    currentRound_++;
    asteroidsToSpawnThisRound_ = calculateAsteroidsForRound(currentRound_);
    asteroidsSpawnedThisRound_ = 0;
    spawnInterval_ = calculateSpawnIntervalForRound(currentRound_);
    showingRoundTransition_ = true;
    roundTransitionTimer_ = 0.0f;
}

int RoundManager::calculateAsteroidsForRound(int round) const {
    return INITIAL_ASTEROIDS + (round - 1) * ASTEROIDS_INCREMENT;
}

float RoundManager::calculateSpawnIntervalForRound(int round) const {
    float interval = INITIAL_SPAWN_INTERVAL - (round - 1) * SPAWN_INTERVAL_DECREASE;
    return std::max(MIN_SPAWN_INTERVAL, interval);
}
