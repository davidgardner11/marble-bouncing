#include "TextRenderer.h"
#include "../core/Config.h"
#include <iostream>
#include <sstream>
#include <iomanip>

TextRenderer::TextRenderer()
    : font(nullptr)
    , fontSize(Config::UI_FONT_SIZE)
    , initialized(false)
{
}

TextRenderer::~TextRenderer() {
    cleanup();
}

bool TextRenderer::initialize() {
    if (initialized) {
        return true;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return false;
    }

    // Try to load a system font
    // macOS common font paths
    const char* fontPaths[] = {
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/Library/Fonts/Arial.ttf",
        "/System/Library/Fonts/SFNSDisplay.ttf"
    };

    for (const char* path : fontPaths) {
        font = TTF_OpenFont(path, fontSize);
        if (font) {
            break;
        }
    }

    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        TTF_Quit();
        return false;
    }

    initialized = true;
    return true;
}

void TextRenderer::cleanup() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (initialized) {
        TTF_Quit();
        initialized = false;
    }
}

void TextRenderer::renderText(
    SDL_Renderer* renderer,
    const std::string& text,
    int x, int y,
    const SDL_Color& color,
    int fontSize)
{
    if (!initialized || !font) {
        return;
    }

    // Create surface from text
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        return;
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    // Render texture
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void TextRenderer::renderFPS(SDL_Renderer* renderer, float fps, int x, int y) {
    std::ostringstream oss;
    oss << "FPS: " << std::fixed << std::setprecision(1) << fps;
    renderText(renderer, oss.str(), x, y, Config::TEXT_COLOR);
}

void TextRenderer::renderBallCount(SDL_Renderer* renderer, size_t count, int x, int y) {
    std::ostringstream oss;
    oss << "Balls: " << count;
    renderText(renderer, oss.str(), x, y, Config::TEXT_COLOR);
}

void TextRenderer::renderTimer(SDL_Renderer* renderer, float elapsedTime, int x, int y) {
    // Format time as MM:SS.ms
    int minutes = static_cast<int>(elapsedTime) / 60;
    int seconds = static_cast<int>(elapsedTime) % 60;
    int milliseconds = static_cast<int>((elapsedTime - static_cast<int>(elapsedTime)) * 100);

    std::ostringstream oss;
    oss << "Time: "
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << "."
        << std::setfill('0') << std::setw(2) << milliseconds;
    renderText(renderer, oss.str(), x, y, Config::TEXT_COLOR);
}
