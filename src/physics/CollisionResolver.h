#pragma once

#include "../entities/Ball.h"
#include "CollisionDetector.h"

class CollisionResolver {
public:
    // Resolve elastic collision between two balls
    static void resolveElasticCollision(Ball& a, Ball& b, const CollisionInfo& info, float restitution = 1.0f);

    // Resolve ball-wall collision
    static void resolveWallCollision(Ball& ball, const CollisionInfo& info, float restitution = 1.0f);

private:
    // Separate overlapping balls
    static void separateBalls(Ball& a, Ball& b, float penetration, const Vector2D& normal);
};
