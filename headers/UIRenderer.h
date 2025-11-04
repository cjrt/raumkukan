#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class UIRenderer {
public:

    UIRenderer(SDL_Renderer* renderer, const std::string& fontPath, 
               int normalFontSize, int largeFontSize);
    
    ~UIRenderer();

    void renderText(const std::string& text, int x, int y, 
                    int fontSize, const SDL_Color& color);

    void renderTextCentered(const std::string& text, int centerX, int centerY,
                           int fontSize, const SDL_Color& color);

    bool isInitialized() const { return initialized_; }

private:
    SDL_Renderer* renderer_;
    TTF_Font* normalFont_;
    TTF_Font* largeFont_;
    bool initialized_;
    
    void renderTextWithFont(const std::string& text, int x, int y,
                           TTF_Font* font, const SDL_Color& color);
};

#endif 
