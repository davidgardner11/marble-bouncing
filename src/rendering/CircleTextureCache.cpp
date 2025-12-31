#include "CircleTextureCache.h"
#include <cmath>

CircleTextureCache::CircleTextureCache(SDL_Renderer* renderer)
    : renderer(renderer)
{
}

CircleTextureCache::~CircleTextureCache() {
    cleanup();
}

SDL_Texture* CircleTextureCache::getCircleTexture(const SDL_Color& color, float radius) {
    uint32_t key = colorToKey(color, radius);

    auto it = cache.find(key);
    if (it != cache.end()) {
        return it->second;
    }

    int diameter = static_cast<int>(radius * 2);
    SDL_Texture* texture = createCircleTexture(color, diameter);
    cache[key] = texture;
    return texture;
}

SDL_Texture* CircleTextureCache::createCircleTexture(const SDL_Color& color, int diameter) {
    // Create texture
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        diameter,
        diameter
    );

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // Render circle to texture
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Draw filled circle using midpoint algorithm
    float radius = diameter / 2.0f;
    int cx = diameter / 2;
    int cy = diameter / 2;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int y = 0; y < diameter; ++y) {
        for (int x = 0; x < diameter; ++x) {
            float dx = x - cx;
            float dy = y - cy;
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);

    return texture;
}

void CircleTextureCache::cleanup() {
    for (auto& pair : cache) {
        SDL_DestroyTexture(pair.second);
    }
    cache.clear();
}

uint32_t CircleTextureCache::colorToKey(const SDL_Color& color, float radius) const {
    // Pack color and radius into key
    uint32_t key = (static_cast<uint32_t>(color.r) << 24) |
                   (static_cast<uint32_t>(color.g) << 16) |
                   (static_cast<uint32_t>(color.b) << 8) |
                   (static_cast<uint32_t>(radius));
    return key;
}
