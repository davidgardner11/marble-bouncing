#include "Button.h"

Button::Button(int x, int y, int width, int height, const std::string& label)
    : x(x)
    , y(y)
    , width(width)
    , height(height)
    , label(label)
    , hovered(false)
    , pressed(false)
    , onClick(nullptr)
{
}

void Button::handleMouseDown(int mouseX, int mouseY) {
    if (containsPoint(mouseX, mouseY)) {
        pressed = true;
    }
}

void Button::handleMouseUp(int mouseX, int mouseY) {
    if (pressed && containsPoint(mouseX, mouseY)) {
        // Button was clicked
        if (onClick) {
            onClick();
        }
    }
    pressed = false;
}

void Button::handleMouseMove(int mouseX, int mouseY) {
    hovered = containsPoint(mouseX, mouseY);
}

void Button::render(SDL_Renderer* renderer) {
    // Choose button color based on state
    SDL_Color buttonColor;
    if (pressed) {
        buttonColor = {100, 100, 150, 255};  // Darker when pressed
    } else if (hovered) {
        buttonColor = {140, 140, 180, 255};  // Lighter when hovered
    } else {
        buttonColor = {120, 120, 160, 255};  // Normal state
    }

    // Draw button background
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_Rect buttonRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &buttonRect);

    // Draw button border
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &buttonRect);

    // Label will be rendered by TextRenderer in Application
}

bool Button::containsPoint(int px, int py) const {
    return px >= x && px <= x + width && py >= y && py <= y + height;
}
