#pragma once

#include <SDL2/SDL.h>
#include "../math/Vector2D.h"
#include "CircleTextureCache.h"

class CircleRenderer {
public:
    CircleRenderer();
    ~CircleRenderer();

    void initialize(SDL_Renderer* renderer);
    void cleanup();

    // Optimized texture-based rendering
    void drawFilledCircleFast(
        SDL_Renderer* renderer,
        const Vector2D& center,
        float radius,
        const SDL_Color& color
    );


    // Draw filled circle
    static void drawFilledCircle(
        SDL_Renderer* renderer,
        const Vector2D& center,
        float radius,
        const SDL_Color& color
    );

    // Draw circle outline
    static void drawCircleOutline(
        SDL_Renderer* renderer,
        const Vector2D& center,
        float radius,
        const SDL_Color& color,
        int thickness = 1
    );

    // Draw arc (for container with gap)
    // Angles in radians, measured from positive X-axis (0 = right, π/2 = down)
    static void drawArc(
        SDL_Renderer* renderer,
        const Vector2D& center,
        float radius,
        float startAngleRad,
        float endAngleRad,
        const SDL_Color& color,
        int thickness = 2
    );

private:
    CircleTextureCache* textureCache;

    // Midpoint circle algorithm helper
    static void plotCirclePoints(
        SDL_Renderer* renderer,
        int centerX, int centerY,
        int x, int y,
        bool filled
    );

    // Plot single pixel
    static void plotPoint(SDL_Renderer* renderer, int x, int y);
};
