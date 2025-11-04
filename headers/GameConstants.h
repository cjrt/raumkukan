#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

// defines games constants

namespace GameConstants {
    // dimensions
    constexpr int SCREEN_WIDTH = 1920;
    constexpr int SCREEN_HEIGHT = 1080;

    // round variables
    constexpr int INITIAL_ASTEROIDS_PER_ROUND = 10;
    constexpr int ASTEROIDS_INCREMENT_PER_ROUND = 5;
    constexpr float INITIAL_SPAWN_INTERVAL = 0.75f;
    constexpr float MIN_SPAWN_INTERVAL = 0.3f;
    constexpr float SPAWN_INTERVAL_DECREASE_PER_ROUND = 0.05f;
    constexpr float ROUND_TRANSITION_DURATION = 2.0f;
    constexpr int ASTEROIDS_PER_SPAWN_EVENT = 2;

    // font paths and variables
    constexpr const char* FONT_PATH = "assets/menufont.ttf";
    constexpr int ROUND_DISPLAY_FONT_SIZE = 36;
    constexpr int ROUND_TRANSITION_FONT_SIZE = 72;

    // asset paths
    constexpr const char* PLAYER_TEXTURE_PATH = "assets/player.png";
    constexpr const char* ASTEROID_TEXTURE_PATH = "assets/asteroid.png";
    constexpr const char* ASTEROID2_TEXTURE_PATH = "assets/asteroid2.png";
    constexpr const char* BACKGROUND_FOLDER_PATH = "assets/background";
    constexpr const char* MUSIC_PATH = "assets/backgroundmusic.mp3";
    constexpr const char* GUN_SOUND_PATH = "assets/gunsound.mp3";

    // settings
    constexpr float PLAYER_SCALE = 0.20f;
    constexpr int BACKGROUND_FRAME_COUNT = 9;
    constexpr int BACKGROUND_FRAME_DELAY_MS = 100;

    // UI positioning
    constexpr int ROUND_TEXT_X_OFFSET = 20;
    constexpr int ROUND_TEXT_Y_OFFSET = 20;
}

#endif
