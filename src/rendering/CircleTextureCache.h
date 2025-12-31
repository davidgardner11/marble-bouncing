#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

class CircleTextureCache {
public:
    CircleTextureCache(SDL_Renderer* renderer);
    ~CircleTextureCache();

    // Get or create circle texture
    SDL_Texture* getCircleTexture(const SDL_Color& color, float radius);

    void cleanup();

private:
    SDL_Renderer* renderer;
    std::unordered_map<uint32_t, SDL_Texture*> cache;

    uint32_t colorToKey(const SDL_Color& color, float radius) const;
    SDL_Texture* createCircleTexture(const SDL_Color& color, int diameter);
};
