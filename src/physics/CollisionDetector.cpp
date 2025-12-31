#include "CollisionDetector.h"
#include "../math/MathUtils.h"
#include <cmath>

CollisionInfo CollisionDetector::checkBallCollision(const Ball& a, const Ball& b) {
    CollisionInfo info;

    // Calculate distance between ball centers
    Vector2D delta = b.position - a.position;
    float distanceSquared = delta.magnitudeSquared();
    float combinedRadius = a.radius + b.radius;
    float combinedRadiusSquared = combinedRadius * combinedRadius;

    // Check if balls are overlapping
    if (distanceSquared < combinedRadiusSquared && distanceSquared > 0.0001f) {
        info.hasCollision = true;
        float distance = std::sqrt(distanceSquared);
        info.normal = delta / distance;  // Normalize
        info.penetration = combinedRadius - distance;
    }

    return info;
}

CollisionInfo CollisionDetector::checkContainerCollision(
    const Ball& ball,
    const Container& container)
{
    CollisionInfo info;

    // Calculate distance from ball center to container center
    Vector2D delta = ball.position - container.getCenter();
    float distance = delta.magnitude();

    // Check if ball is outside container bounds
    float containerInnerRadius = container.getRadius() - ball.radius;

    if (distance > containerInnerRadius) {
        // Ball is outside container, check if it's hitting the wall or in the gap
        float angle = std::atan2(delta.y, delta.x);
        angle = MathUtils::normalizeAngle(angle);

        // Check if collision point is in the gap
        float gapStart = container.getGapStartAngle();
        float gapEnd = container.getGapEndAngle();

        if (!MathUtils::isAngleInRange(angle, gapStart, gapEnd)) {
            // Not in gap, so there's a collision with the container wall
            info.hasCollision = true;
            info.normal = delta.normalized();  // Normal points outward from center
            info.penetration = distance - containerInnerRadius;
        }
    }

    return info;
}

bool CollisionDetector::isAngleInGap(float angle, float gapStart, float gapEnd) {
    return MathUtils::isAngleInRange(angle, gapStart, gapEnd);
}

float CollisionDetector::getAngleFromCenter(const Vector2D& point, const Vector2D& center) {
    return std::atan2(point.y - center.y, point.x - center.x);
}
