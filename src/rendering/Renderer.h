#pragma once

#include <SDL2/SDL.h>
#include <string>

class Renderer {
public:
    Renderer(int windowWidth, int windowHeight, const std::string& title);
    ~Renderer();

    // Initialization
    bool initialize();
    void cleanup();

    // Frame management
    void beginFrame();
    void endFrame();

    // Drawing primitives
    void clear(const SDL_Color& color);
    void setDrawColor(const SDL_Color& color);

    // Get window dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Access SDL renderer (for specialized rendering)
    SDL_Renderer* getSDLRenderer() { return renderer; }

    bool isInitialized() const { return initialized; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
    std::string title;
    bool initialized;
};
