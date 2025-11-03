#ifndef START_SCREEN_H
#define START_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>

class StartScreen {
public:
    StartScreen(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~StartScreen();
    
    void handleEvent(const SDL_Event& e);
    void render();
    bool shouldStartGame() const { return startGame_; }
    void reset() { startGame_ = false; }
    
private:
    SDL_Renderer* renderer_;
    SDL_Texture* playButtonTexture_;
    SDL_Texture* playerTexture_;
    SDL_Rect playButtonRect_;
    SDL_Rect playerRect_;
    TTF_Font* font_;
    int screenWidth_;
    int screenHeight_;
    int playerWidth_;
    int playerHeight_;
    float playerScale_;
    bool mouseOverButton_;
    bool startGame_;
    
    void createPlayButton();
    void loadPlayerTexture();
    float getAngleToMouse() const;
    bool isMouseOverButton(int mouseX, int mouseY) const;
};

#endif
