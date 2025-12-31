#pragma once

#include <SDL2/SDL.h>
#include <string>

class Slider {
public:
    Slider(int x, int y, int width, int height, float minValue, float maxValue, float initialValue);

    // Handle mouse events
    void handleMouseDown(int mouseX, int mouseY);
    void handleMouseUp();
    void handleMouseMove(int mouseX, int mouseY);

    // Render the slider
    void render(SDL_Renderer* renderer, const std::string& label);

    // Get/Set value
    float getValue() const { return value; }
    void setValue(float newValue);

    // Check if point is inside slider
    bool containsPoint(int x, int y) const;

private:
    int x, y;
    int width, height;
    float minValue, maxValue;
    float value;
    bool dragging;

    // Calculate slider position from value
    int getHandleX() const;

    // Calculate value from mouse position
    float valueFromX(int mouseX) const;
};
