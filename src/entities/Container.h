#pragma once

#include "../math/Vector2D.h"

class Container {
public:
    Container(const Vector2D& center, float radius, float gapAngleDegrees);

    // Update rotation
    void update(float deltaTime);

    // Collision detection helpers
    bool isPointInsideContainer(const Vector2D& point) const;
    bool isPointInGap(const Vector2D& point) const;

    // Get gap boundaries for collision detection (in radians)
    float getGapStartAngle() const;
    float getGapEndAngle() const;

    // Rendering info
    Vector2D getCenter() const { return center; }
    float getRadius() const { return radius; }
    float getCurrentRotation() const { return currentAngleRad; }
    float getGapAngleDegrees() const { return gapAngleDegrees; }

private:
    Vector2D center;
    float radius;
    float gapAngleDegrees;     // Size of gap in degrees
    float rotationSpeed;        // Degrees per second
    float currentAngleRad;      // Current rotation angle in radians
};
