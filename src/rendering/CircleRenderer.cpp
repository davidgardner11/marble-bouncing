#include "CircleRenderer.h"
#include "../math/MathUtils.h"
#include <cmath>

CircleRenderer::CircleRenderer()
    : textureCache(nullptr)
{
}

CircleRenderer::~CircleRenderer() {
    cleanup();
}

void CircleRenderer::initialize(SDL_Renderer* renderer) {
    textureCache = new CircleTextureCache(renderer);
}

void CircleRenderer::cleanup() {
    if (textureCache) {
        delete textureCache;
        textureCache = nullptr;
    }
}

void CircleRenderer::drawFilledCircleFast(
    SDL_Renderer* renderer,
    const Vector2D& center,
    float radius,
    const SDL_Color& color)
{
    SDL_Texture* texture = textureCache->getCircleTexture(color, radius);

    int diameter = static_cast<int>(radius * 2);
    SDL_Rect destRect = {
        static_cast<int>(center.x - radius),
        static_cast<int>(center.y - radius),
        diameter,
        diameter
    };

    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void CircleRenderer::drawFilledCircle(
    SDL_Renderer* renderer,
    const Vector2D& center,
    float radius,
    const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int cx = static_cast<int>(center.x);
    int cy = static_cast<int>(center.y);
    int r = static_cast<int>(radius);

    // Midpoint circle algorithm with filled scanlines
    int x = 0;
    int y = r;
    int d = 1 - r;

    while (x <= y) {
        // Draw horizontal lines for filled circle
        SDL_RenderDrawLine(renderer, cx - x, cy + y, cx + x, cy + y);
        SDL_RenderDrawLine(renderer, cx - x, cy - y, cx + x, cy - y);
        SDL_RenderDrawLine(renderer, cx - y, cy + x, cx + y, cy + x);
        SDL_RenderDrawLine(renderer, cx - y, cy - x, cx + y, cy - x);

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void CircleRenderer::drawCircleOutline(
    SDL_Renderer* renderer,
    const Vector2D& center,
    float radius,
    const SDL_Color& color,
    int thickness)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int t = 0; t < thickness; ++t) {
        int cx = static_cast<int>(center.x);
        int cy = static_cast<int>(center.y);
        int r = static_cast<int>(radius) - t;

        if (r <= 0) break;

        int x = 0;
        int y = r;
        int d = 1 - r;

        while (x <= y) {
            plotCirclePoints(renderer, cx, cy, x, y, false);

            if (d < 0) {
                d += 2 * x + 3;
            } else {
                d += 2 * (x - y) + 5;
                y--;
            }
            x++;
        }
    }
}

void CircleRenderer::drawArc(
    SDL_Renderer* renderer,
    const Vector2D& center,
    float radius,
    float startAngleRad,
    float endAngleRad,
    const SDL_Color& color,
    int thickness)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Check if this is a full circle (arc spans >= 2π)
    float arcSpan = endAngleRad - startAngleRad;
    bool isFullCircle = arcSpan >= MathUtils::TWO_PI || MathUtils::floatEquals(arcSpan, MathUtils::TWO_PI);

    // Normalize angles only if not a full circle
    if (!isFullCircle) {
        startAngleRad = MathUtils::normalizeAngle(startAngleRad);
        endAngleRad = MathUtils::normalizeAngle(endAngleRad);
    }

    // Draw arc by plotting points along the circumference
    int numPoints = static_cast<int>(radius * 4);  // More points for smoother arcs
    float angleStep = MathUtils::TWO_PI / numPoints;

    for (int i = 0; i <= numPoints; ++i) {
        float angle = i * angleStep;

        // Check if this angle is within the arc range
        bool inRange;
        if (isFullCircle) {
            // Full circle - always draw
            inRange = true;
        } else if (startAngleRad < endAngleRad) {
            inRange = (angle >= startAngleRad && angle <= endAngleRad);
        } else {
            // Wrap-around case
            inRange = (angle >= startAngleRad || angle <= endAngleRad);
        }

        if (inRange) {
            for (int t = 0; t < thickness; ++t) {
                float r = radius - t;
                int x = static_cast<int>(center.x + r * std::cos(angle));
                int y = static_cast<int>(center.y + r * std::sin(angle));
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

void CircleRenderer::plotCirclePoints(
    SDL_Renderer* renderer,
    int centerX, int centerY,
    int x, int y,
    bool filled)
{
    if (filled) {
        // Draw horizontal lines for filled circle
        SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX + x, centerY + y);
        SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX + x, centerY - y);
        SDL_RenderDrawLine(renderer, centerX - y, centerY + x, centerX + y, centerY + x);
        SDL_RenderDrawLine(renderer, centerX - y, centerY - x, centerX + y, centerY - x);
    } else {
        // Plot 8 symmetric points
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
    }
}

void CircleRenderer::plotPoint(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}
