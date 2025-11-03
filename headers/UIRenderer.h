#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

/**
 * @class UIRenderer
 * @brief Handles all UI text rendering operations
 * 
 * This class encapsulates text rendering functionality, making it easy to
 * display text on screen with proper font management and rendering.
 */
class UIRenderer {
public:
    /**
     * @brief Constructor - initializes fonts
     * @param renderer SDL renderer to use for rendering
     * @param fontPath Path to the TTF font file
     * @param normalFontSize Size for normal UI text
     * @param largeFontSize Size for large UI text (e.g., transitions)
     */
    UIRenderer(SDL_Renderer* renderer, const std::string& fontPath, 
               int normalFontSize, int largeFontSize);
    
    /**
     * @brief Destructor - cleans up fonts
     */
    ~UIRenderer();

    /**
     * @brief Renders text at a specific position
     * @param text Text to render
     * @param x X position on screen
     * @param y Y position on screen
     * @param fontSize Size of font (0 = normal, 1 = large)
     * @param color SDL color for the text
     */
    void renderText(const std::string& text, int x, int y, 
                    int fontSize, const SDL_Color& color);

    /**
     * @brief Renders text centered on screen
     * @param text Text to render
     * @param centerX X center position
     * @param centerY Y center position
     * @param fontSize Size of font (0 = normal, 1 = large)
     * @param color SDL color for the text
     */
    void renderTextCentered(const std::string& text, int centerX, int centerY,
                           int fontSize, const SDL_Color& color);

    /**
     * @brief Checks if the renderer was successfully initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const { return initialized_; }

private:
    SDL_Renderer* renderer_;
    TTF_Font* normalFont_;
    TTF_Font* largeFont_;
    bool initialized_;
    
    /**
     * @brief Internal helper to render text with a specific font
     */
    void renderTextWithFont(const std::string& text, int x, int y,
                           TTF_Font* font, const SDL_Color& color);
};

#endif // UI_RENDERER_H
