#pragma once

#include "../entities/Ball.h"
#include "../entities/Container.h"
#include "CollisionDetector.h"
#include "CollisionResolver.h"
#include "SpatialGrid.h"
#include <vector>

class PhysicsEngine {
public:
    PhysicsEngine(float gravity);

    // Main physics update
    void update(std::vector<Ball>& balls, const Container& container, float deltaTime);

    // Configuration
    void setGravity(float gravity) { this->gravity = gravity; }
    float getGravity() const { return gravity; }

private:
    float gravity;  // Pixels per second²
    CollisionDetector detector;
    CollisionResolver resolver;
    SpatialGrid spatialGrid;
    std::vector<std::pair<size_t, size_t>> potentialCollisions;

    // Update steps
    void applyGravity(std::vector<Ball>& balls, float deltaTime);
    void updatePositions(std::vector<Ball>& balls, float deltaTime);
    void handleCollisions(std::vector<Ball>& balls, const Container& container);
    void handleBallBallCollisions(std::vector<Ball>& balls);
    void handleBallContainerCollisions(std::vector<Ball>& balls, const Container& container);
};
