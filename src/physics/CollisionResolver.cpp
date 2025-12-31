#include "CollisionResolver.h"

void CollisionResolver::resolveElasticCollision(Ball& a, Ball& b, const CollisionInfo& info) {
    if (!info.hasCollision) {
        return;
    }

    // Get collision normal
    Vector2D normal = info.normal;

    // Calculate relative velocity
    Vector2D relativeVelocity = b.velocity - a.velocity;

    // Calculate velocity along the normal
    float velocityAlongNormal = relativeVelocity.dot(normal);

    // Don't resolve if balls are separating
    if (velocityAlongNormal > 0.0f) {
        return;
    }

    // Calculate new velocities using elastic collision formula
    // v1' = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2)
    // v2' = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2)

    float m1 = a.mass;
    float m2 = b.mass;
    float totalMass = m1 + m2;

    // Project velocities onto collision normal
    float v1n = a.velocity.dot(normal);
    float v2n = b.velocity.dot(normal);

    // Calculate new normal velocities (elastic collision)
    float v1n_new = ((m1 - m2) * v1n + 2.0f * m2 * v2n) / totalMass;
    float v2n_new = ((m2 - m1) * v2n + 2.0f * m1 * v1n) / totalMass;

    // Update velocities
    a.velocity += normal * (v1n_new - v1n);
    b.velocity += normal * (v2n_new - v2n);

    // Separate balls to prevent overlap
    separateBalls(a, b, info.penetration, normal);
}

void CollisionResolver::resolveWallCollision(Ball& ball, const CollisionInfo& info) {
    if (!info.hasCollision) {
        return;
    }

    // Get collision normal (points outward from container center)
    Vector2D normal = info.normal;

    // Calculate velocity along the normal
    float velocityAlongNormal = ball.velocity.dot(normal);

    // Don't resolve if ball is moving away from wall
    if (velocityAlongNormal < 0.0f) {
        return;
    }

    // Reflect velocity across normal (100% restitution)
    ball.velocity -= normal * (2.0f * velocityAlongNormal);

    // Position correction: push ball back inside container
    ball.position -= normal * info.penetration;
}

void CollisionResolver::separateBalls(Ball& a, Ball& b, float penetration, const Vector2D& normal) {
    // Separate balls based on their mass ratio
    float totalMass = a.mass + b.mass;
    float separationA = penetration * (b.mass / totalMass);
    float separationB = penetration * (a.mass / totalMass);

    a.position -= normal * separationA;
    b.position += normal * separationB;
}
