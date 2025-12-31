#pragma once

#include "../math/Vector2D.h"
#include "../entities/Ball.h"
#include "../entities/Container.h"

struct CollisionInfo {
    bool hasCollision;
    Vector2D normal;        // Collision normal
    float penetration;      // How deep the collision is

    CollisionInfo()
        : hasCollision(false)
        , normal(0.0f, 0.0f)
        , penetration(0.0f)
    {}
};

class CollisionDetector {
public:
    // Ball-Ball collision detection
    static CollisionInfo checkBallCollision(const Ball& a, const Ball& b);

    // Ball-Container collision detection (excluding gap)
    static CollisionInfo checkContainerCollision(
        const Ball& ball,
        const Container& container
    );

private:
    // Helper: Check if angle is within gap range
    static bool isAngleInGap(float angle, float gapStart, float gapEnd);

    // Helper: Get angle of point relative to container center
    static float getAngleFromCenter(const Vector2D& point, const Vector2D& center);
};
