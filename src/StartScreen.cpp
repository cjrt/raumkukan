#include "../headers/StartScreen.h"
#include "../headers/GameConstants.h"
#include <iostream>
#include <cmath>
#include <algorithm>

StartScreen::StartScreen(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer_(renderer), playButtonTexture_(nullptr), playerTexture_(nullptr),
      font_(nullptr), screenWidth_(screenWidth), screenHeight_(screenHeight),
      playerScale_(1.2f), mouseOverButton_(false), startGame_(false) {
    
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return;
    }
    
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
    
    SDL_Color textColor = {255, 255, 255, 255}; // White text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, "PLAY", textColor);
    if (!textSurface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }
    
    playButtonTexture_ = SDL_CreateTextureFromSurface(renderer_, textSurface);
    SDL_FreeSurface(textSurface);
    
    if (!playButtonTexture_) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return;
    }
    
    int textWidth, textHeight;
    SDL_QueryTexture(playButtonTexture_, nullptr, nullptr, &textWidth, &textHeight);
    
    // play button position
    int rightSectionWidth = screenWidth_ / 2; 
    int buttonMargin = 50; 
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

    int leftSectionWidth = screenWidth_ / 2; 
    int leftSectionHeight = screenHeight_;
    
    float maxWidthScale = (leftSectionWidth * 0.9f) / surface->w;
    float maxHeightScale = (leftSectionHeight * 0.9f) / surface->h;
    float fitScale = std::min(maxWidthScale, maxHeightScale);
    
    // Use the larger of fitScale or playerScale_
    float finalScale = std::max(playerScale_, fitScale);
    
    playerWidth_ = static_cast<int>(surface->w * finalScale);
    playerHeight_ = static_cast<int>(surface->h * finalScale);
    SDL_FreeSurface(surface);

    // player position
    int playerMargin = (leftSectionWidth - playerWidth_) / 2; 
    playerRect_.x = playerMargin;
    playerRect_.y = (screenHeight_ - playerHeight_) / 2;
    playerRect_.w = playerWidth_;
    playerRect_.h = playerHeight_;
}

float StartScreen::getAngleToMouse() const {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    float playerCenterX = playerRect_.x + playerRect_.w / 2.0f;
    float playerCenterY = playerRect_.y + playerRect_.h / 2.0f;
    
    float dx = mouseX - playerCenterX;
    float dy = mouseY - playerCenterY;
    
    float angle = atan2f(dy, dx) * 180.0f / M_PI;
    // 90 degrees offset to match 
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
    // player sprite rotated toward mouse
    if (playerTexture_) {
        float angle = getAngleToMouse();
        SDL_RenderCopyEx(renderer_, playerTexture_, nullptr, &playerRect_,
                        angle, nullptr, SDL_FLIP_NONE);
    }
    
    if (!playButtonTexture_) return;
    
    SDL_RenderCopy(renderer_, playButtonTexture_, nullptr, &playButtonRect_);
}

bool StartScreen::isMouseOverButton(int mouseX, int mouseY) const {
    return (mouseX >= playButtonRect_.x && 
            mouseX <= playButtonRect_.x + playButtonRect_.w &&
            mouseY >= playButtonRect_.y && 
            mouseY <= playButtonRect_.y + playButtonRect_.h);
}
