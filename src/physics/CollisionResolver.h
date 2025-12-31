#pragma once

#include "../entities/Ball.h"
#include "CollisionDetector.h"

class CollisionResolver {
public:
    // Resolve elastic collision between two balls
    static void resolveElasticCollision(Ball& a, Ball& b, const CollisionInfo& info);

    // Resolve ball-wall collision (100% bounce)
    static void resolveWallCollision(Ball& ball, const CollisionInfo& info);

private:
    // Separate overlapping balls
    static void separateBalls(Ball& a, Ball& b, float penetration, const Vector2D& normal);
};
