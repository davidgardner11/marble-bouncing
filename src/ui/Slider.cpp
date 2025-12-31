#include "Slider.h"
#include <algorithm>

Slider::Slider(int x, int y, int width, int height, float minValue, float maxValue, float initialValue)
    : x(x)
    , y(y)
    , width(width)
    , height(height)
    , minValue(minValue)
    , maxValue(maxValue)
    , value(initialValue)
    , dragging(false)
{
}

void Slider::handleMouseDown(int mouseX, int mouseY) {
    if (containsPoint(mouseX, mouseY)) {
        dragging = true;
        value = valueFromX(mouseX);
    }
}

void Slider::handleMouseUp() {
    dragging = false;
}

void Slider::handleMouseMove(int mouseX, int) {
    if (dragging) {
        value = valueFromX(mouseX);
    }
}

void Slider::render(SDL_Renderer* renderer, const std::string& label) {
    // Draw label above slider
    // (Text rendering will be handled by TextRenderer in Application)

    // Draw slider track
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect trackRect = {x, y + height / 2 - 2, width, 4};
    SDL_RenderFillRect(renderer, &trackRect);

    // Draw slider handle
    int handleX = getHandleX();
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect handleRect = {handleX - 6, y, 12, height};
    SDL_RenderFillRect(renderer, &handleRect);

    // Draw handle outline
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &handleRect);
}

void Slider::setValue(float newValue) {
    value = std::clamp(newValue, minValue, maxValue);
}

bool Slider::containsPoint(int px, int py) const {
    return px >= x && px <= x + width && py >= y && py <= y + height;
}

int Slider::getHandleX() const {
    float normalized = (value - minValue) / (maxValue - minValue);
    return x + static_cast<int>(normalized * width);
}

float Slider::valueFromX(int mouseX) const {
    // Clamp mouse position to slider bounds
    int clampedX = std::clamp(mouseX, x, x + width);

    // Calculate normalized position (0 to 1)
    float normalized = static_cast<float>(clampedX - x) / static_cast<float>(width);

    // Convert to value range
    return minValue + normalized * (maxValue - minValue);
}
