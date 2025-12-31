#include "Container.h"
#include "../math/MathUtils.h"
#include <cmath>

Container::Container(const Vector2D& center, float radius, float gapAngleDegrees)
    : center(center)
    , radius(radius)
    , gapAngleDegrees(gapAngleDegrees)
    , rotationSpeed(360.0f / 10.0f)  // 360° / 10 seconds = 36°/s
    , currentAngleRad(0.0f)
{
}

void Container::update(float deltaTime) {
    // Rotate at 36° per second (full rotation in 10 seconds)
    float deltaAngleDeg = rotationSpeed * deltaTime;
    currentAngleRad += MathUtils::degToRad(deltaAngleDeg);

    // Keep angle in [0, 2π] range
    currentAngleRad = MathUtils::normalizeAngle(currentAngleRad);
}

bool Container::isPointInsideContainer(const Vector2D& point) const {
    float distance = center.distance(point);
    return distance < radius;
}

bool Container::isPointInGap(const Vector2D& point) const {
    // Calculate angle of point relative to container center
    float angle = std::atan2(point.y - center.y, point.x - center.x);
    angle = MathUtils::normalizeAngle(angle);

    // Get gap boundaries
    float gapStart = getGapStartAngle();
    float gapEnd = getGapEndAngle();

    // Check if angle is within gap range
    return MathUtils::isAngleInRange(angle, gapStart, gapEnd);
}

float Container::getGapStartAngle() const {
    return currentAngleRad;
}

float Container::getGapEndAngle() const {
    // Gap size in radians
    float gapAngleRad = MathUtils::degToRad(gapAngleDegrees);
    return currentAngleRad + gapAngleRad;
}
