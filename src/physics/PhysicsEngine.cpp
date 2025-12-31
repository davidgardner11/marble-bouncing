#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine(float gravity)
    : gravity(gravity)
    , spatialGrid(50.0f, 1024.0f, 768.0f)  // Cell size = 2 × ball diameter
{
}

void PhysicsEngine::update(std::vector<Ball>& balls, const Container& container, float deltaTime, float restitution) {
    // Apply gravity to all balls
    applyGravity(balls, deltaTime);

    // Update ball positions based on velocity
    updatePositions(balls, deltaTime);

    // Handle all collisions
    handleCollisions(balls, container, restitution);
}

void PhysicsEngine::applyGravity(std::vector<Ball>& balls, float deltaTime) {
    for (Ball& ball : balls) {
        ball.applyGravity(gravity, deltaTime);
    }
}

void PhysicsEngine::updatePositions(std::vector<Ball>& balls, float deltaTime) {
    for (Ball& ball : balls) {
        ball.update(deltaTime);
    }
}

void PhysicsEngine::handleCollisions(std::vector<Ball>& balls, const Container& container, float restitution) {
    // Handle ball-ball collisions
    handleBallBallCollisions(balls, restitution);

    // Handle ball-container collisions
    handleBallContainerCollisions(balls, container, restitution);
}

void PhysicsEngine::handleBallBallCollisions(std::vector<Ball>& balls, float restitution) {
    // Rebuild spatial grid
    spatialGrid.clear();
    for (size_t i = 0; i < balls.size(); ++i) {
        spatialGrid.insertBall(i, balls[i].position);
    }

    // Get potential collision pairs
    spatialGrid.getPotentialCollisions(balls, potentialCollisions);

    // Check only potential collisions
    for (const auto& pair : potentialCollisions) {
        CollisionInfo info = detector.checkBallCollision(balls[pair.first], balls[pair.second]);
        if (info.hasCollision) {
            resolver.resolveElasticCollision(balls[pair.first], balls[pair.second], info, restitution);
        }
    }
}

void PhysicsEngine::handleBallContainerCollisions(std::vector<Ball>& balls, const Container& container, float restitution) {
    for (Ball& ball : balls) {
        CollisionInfo info = detector.checkContainerCollision(ball, container);
        if (info.hasCollision) {
            resolver.resolveWallCollision(ball, info, restitution);
        }
    }
}
