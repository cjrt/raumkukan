#include "../headers/StartScreen.h"
#include "../headers/GameConstants.h"
#include <iostream>
#include <cmath>
#include <algorithm>

StartScreen::StartScreen(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer_(renderer), playButtonTexture_(nullptr), playerTexture_(nullptr),
      font_(nullptr), screenWidth_(screenWidth), screenHeight_(screenHeight),
      playerScale_(1.2f), mouseOverButton_(false), startGame_(false) {
    
    // Initialize TTF
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return;
    }
    
    // Load font with larger size for bigger button
    font_ = TTF_OpenFont(GameConstants::FONT_PATH, 200);
    if (!font_) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }
    
    loadPlayerTexture();
    createPlayButton();
}

StartScreen::~StartScreen() {
    if (playButtonTexture_) {
        SDL_DestroyTexture(playButtonTexture_);
    }
    if (playerTexture_) {
        SDL_DestroyTexture(playerTexture_);
    }
    if (font_) {
        TTF_CloseFont(font_);
    }
    TTF_Quit();
}

void StartScreen::createPlayButton() {
    if (!font_) return;
    
    // Create text surface
    SDL_Color textColor = {255, 255, 255, 255}; // White text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, "PLAY", textColor);
    if (!textSurface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }
    
    // Create texture from surface
    playButtonTexture_ = SDL_CreateTextureFromSurface(renderer_, textSurface);
    SDL_FreeSurface(textSurface);
    
    if (!playButtonTexture_) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return;
    }
    
    // Set button rectangle - make it large and positioned on right side
    int textWidth, textHeight;
    SDL_QueryTexture(playButtonTexture_, nullptr, nullptr, &textWidth, &textHeight);
    
    // Position play button on the right side, taking up right portion of screen
    int rightSectionWidth = screenWidth_ / 2; // Right half of screen
    int buttonMargin = 50; // Small margin from edges
    playButtonRect_.x = screenWidth_ - rightSectionWidth + buttonMargin;
    playButtonRect_.y = (screenHeight_ - textHeight) / 2;
    playButtonRect_.w = textWidth;
    playButtonRect_.h = textHeight;
}

void StartScreen::loadPlayerTexture() {
    SDL_Surface* surface = IMG_Load(GameConstants::PLAYER_TEXTURE_PATH);
    if (!surface) {
        std::cerr << "Failed to load player image: " << IMG_GetError() << std::endl;
        return;
    }

    playerTexture_ = SDL_CreateTextureFromSurface(renderer_, surface);
    if (!playerTexture_) {
        std::cerr << "Failed to create player texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Calculate player dimensions with scale to fill left portion of screen
    // Make player large enough to take up most of left side
    int leftSectionWidth = screenWidth_ / 2; // Left half of screen
    int leftSectionHeight = screenHeight_;
    
    // Scale player to fit nicely in left section (with some margin)
    float maxWidthScale = (leftSectionWidth * 0.9f) / surface->w;
    float maxHeightScale = (leftSectionHeight * 0.9f) / surface->h;
    float fitScale = std::min(maxWidthScale, maxHeightScale);
    
    // Use the larger of fitScale or playerScale_
    float finalScale = std::max(playerScale_, fitScale);
    
    playerWidth_ = static_cast<int>(surface->w * finalScale);
    playerHeight_ = static_cast<int>(surface->h * finalScale);
    SDL_FreeSurface(surface);

    // Position player on the left side, taking up most of left portion
    int playerMargin = (leftSectionWidth - playerWidth_) / 2; // Center in left section
    playerRect_.x = playerMargin;
    playerRect_.y = (screenHeight_ - playerHeight_) / 2;
    playerRect_.w = playerWidth_;
    playerRect_.h = playerHeight_;
}

float StartScreen::getAngleToMouse() const {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    // Calculate angle from player center to mouse
    float playerCenterX = playerRect_.x + playerRect_.w / 2.0f;
    float playerCenterY = playerRect_.y + playerRect_.h / 2.0f;
    
    float dx = mouseX - playerCenterX;
    float dy = mouseY - playerCenterY;
    
    // Calculate angle in degrees (atan2 returns radians)
    float angle = atan2f(dy, dx) * 180.0f / M_PI;
    
    // Add 90 degrees offset to match game rotation
    return angle + 90.0f;
}

void StartScreen::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        mouseOverButton_ = isMouseOverButton(e.motion.x, e.motion.y);
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        if (isMouseOverButton(e.button.x, e.button.y)) {
            startGame_ = true;
        }
    }
}

void StartScreen::render() {
    // Render player sprite (rotated toward mouse)
    if (playerTexture_) {
        float angle = getAngleToMouse();
        SDL_RenderCopyEx(renderer_, playerTexture_, nullptr, &playerRect_,
                        angle, nullptr, SDL_FLIP_NONE);
    }
    
    // Render play button
    if (!playButtonTexture_) return;
    
    // Set button color based on mouse hover
    if (mouseOverButton_) {
        SDL_SetTextureColorMod(playButtonTexture_, 200, 200, 255); // Light blue when hovering
    } else {
        SDL_SetTextureColorMod(playButtonTexture_, 255, 255, 255); // White normally
    }
    
    SDL_RenderCopy(renderer_, playButtonTexture_, nullptr, &playButtonRect_);
}

bool StartScreen::isMouseOverButton(int mouseX, int mouseY) const {
    return (mouseX >= playButtonRect_.x && 
            mouseX <= playButtonRect_.x + playButtonRect_.w &&
            mouseY >= playButtonRect_.y && 
            mouseY <= playButtonRect_.y + playButtonRect_.h);
}
