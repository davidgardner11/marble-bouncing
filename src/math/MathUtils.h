#pragma once

#include <cmath>
#include <algorithm>

namespace MathUtils {
    // Constants
    constexpr float PI = 3.14159265359f;
    constexpr float TWO_PI = 2.0f * PI;
    constexpr float HALF_PI = PI / 2.0f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;

    // Utility functions
    inline float clamp(float value, float min, float max) {
        return std::max(min, std::min(max, value));
    }

    inline float lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    inline bool floatEquals(float a, float b, float epsilon = 0.0001f) {
        return std::fabs(a - b) < epsilon;
    }

    // Normalize angle to [0, 2π] range
    inline float normalizeAngle(float angleRadians) {
        angleRadians = std::fmod(angleRadians, TWO_PI);
        if (angleRadians < 0.0f) {
            angleRadians += TWO_PI;
        }
        return angleRadians;
    }

    // Check if angle is within range [start, end]
    // Handles wrap-around (e.g., start=350°, end=10°)
    inline bool isAngleInRange(float angle, float start, float end) {
        // Normalize all angles to [0, 2π]
        angle = normalizeAngle(angle);
        start = normalizeAngle(start);
        end = normalizeAngle(end);

        if (start < end) {
            // Normal case: no wrap-around
            return angle >= start && angle <= end;
        } else {
            // Wrap-around case: range crosses 0°
            return angle >= start || angle <= end;
        }
    }

    // Convert degrees to radians
    inline float degToRad(float degrees) {
        return degrees * DEG_TO_RAD;
    }

    // Convert radians to degrees
    inline float radToDeg(float radians) {
        return radians * RAD_TO_DEG;
    }

    // Random float between min and max
    inline float randomRange(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    // Random integer between min and max (inclusive)
    inline int randomRangeInt(int min, int max) {
        return min + (rand() % (max - min + 1));
    }
}
