#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <functional>

class Button {
public:
    Button(int x, int y, int width, int height, const std::string& label);

    // Handle mouse events
    void handleMouseDown(int mouseX, int mouseY);
    void handleMouseUp(int mouseX, int mouseY);
    void handleMouseMove(int mouseX, int mouseY);

    // Render the button
    void render(SDL_Renderer* renderer);

    // Set callback
    void setOnClick(std::function<void()> callback) { onClick = callback; }

    // Check if point is inside button
    bool containsPoint(int x, int y) const;

private:
    int x, y;
    int width, height;
    std::string label;
    bool hovered;
    bool pressed;
    std::function<void()> onClick;
};
