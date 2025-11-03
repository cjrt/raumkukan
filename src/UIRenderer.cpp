#include "../headers/UIRenderer.h"
#include <iostream>

UIRenderer::UIRenderer(SDL_Renderer* renderer, const std::string& fontPath,
                       int normalFontSize, int largeFontSize)
    : renderer_(renderer), normalFont_(nullptr), largeFont_(nullptr), initialized_(false) {
    
    // Load fonts
    normalFont_ = TTF_OpenFont(fontPath.c_str(), normalFontSize);
    if (!normalFont_) {
        std::cerr << "Failed to load normal font: " << TTF_GetError() << std::endl;
        return;
    }

    largeFont_ = TTF_OpenFont(fontPath.c_str(), largeFontSize);
    if (!largeFont_) {
        std::cerr << "Failed to load large font: " << TTF_GetError() << std::endl;
        TTF_CloseFont(normalFont_);
        normalFont_ = nullptr;
        return;
    }

    initialized_ = true;
}

UIRenderer::~UIRenderer() {
    if (normalFont_) {
        TTF_CloseFont(normalFont_);
    }
    if (largeFont_) {
        TTF_CloseFont(largeFont_);
    }
}

void UIRenderer::renderText(const std::string& text, int x, int y,
                           int fontSize, const SDL_Color& color) {
    if (!initialized_) return;

    TTF_Font* font = (fontSize == 0) ? normalFont_ : largeFont_;
    renderTextWithFont(text, x, y, font, color);
}

void UIRenderer::renderTextCentered(const std::string& text, int centerX, int centerY,
                                    int fontSize, const SDL_Color& color) {
    if (!initialized_) return;

    TTF_Font* font = (fontSize == 0) ? normalFont_ : largeFont_;
    
    // Create surface to measure text dimensions
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) return;

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    
    // Calculate position to center the text
    int x = centerX - (textWidth / 2);
    int y = centerY - (textHeight / 2);
    
    SDL_FreeSurface(textSurface);
    renderTextWithFont(text, x, y, font, color);
}

void UIRenderer::renderTextWithFont(const std::string& text, int x, int y,
                                    TTF_Font* font, const SDL_Color& color) {
    if (!font || !renderer_) return;

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer_, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        return;
    }

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
    
    SDL_Rect textRect = {x, y, textWidth, textHeight};
    SDL_RenderCopy(renderer_, textTexture, nullptr, &textRect);
    
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}
