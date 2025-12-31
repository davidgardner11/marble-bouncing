#pragma once

#include "../math/Vector2D.h"
#include <SDL2/SDL.h>
#include <cstdint>

class Ball {
public:
    Ball(const Vector2D& position, const Vector2D& velocity,
         float radius, const SDL_Color& color);

    // Physics properties
    Vector2D position;
    Vector2D velocity;
    float radius;
    float mass;  // Derived from radius (mass = π * r²)

    // Visual properties
    SDL_Color color;
    uint32_t id;  // Unique identifier

    // Physics update
    void update(float deltaTime);
    void applyGravity(float gravity, float deltaTime);

    // Bounds checking
    bool isOffScreen(float screenWidth, float screenHeight) const;

    // Getters
    Vector2D getPosition() const { return position; }
    Vector2D getVelocity() const { return velocity; }
    float getRadius() const { return radius; }
    float getMass() const { return mass; }

private:
    static uint32_t nextId;
    void calculateMass();
};
