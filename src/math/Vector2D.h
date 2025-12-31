#pragma once

#include <cmath>

class Vector2D {
public:
    float x, y;

    // Constructors
    Vector2D() : x(0.0f), y(0.0f) {}
    Vector2D(float x, float y) : x(x), y(y) {}

    // Operators
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator/(float scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2D& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2D& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Vector operations
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    float magnitudeSquared() const {
        return x * x + y * y;
    }

    Vector2D normalized() const {
        float mag = magnitude();
        if (mag > 0.0f) {
            return Vector2D(x / mag, y / mag);
        }
        return Vector2D(0.0f, 0.0f);
    }

    void normalize() {
        float mag = magnitude();
        if (mag > 0.0f) {
            x /= mag;
            y /= mag;
        }
    }

    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    float distance(const Vector2D& other) const {
        return (*this - other).magnitude();
    }

    float distanceSquared(const Vector2D& other) const {
        return (*this - other).magnitudeSquared();
    }

    // Rotation
    Vector2D rotated(float angleRadians) const {
        float cosA = std::cos(angleRadians);
        float sinA = std::sin(angleRadians);
        return Vector2D(
            x * cosA - y * sinA,
            x * sinA + y * cosA
        );
    }

    void rotate(float angleRadians) {
        float cosA = std::cos(angleRadians);
        float sinA = std::sin(angleRadians);
        float newX = x * cosA - y * sinA;
        float newY = x * sinA + y * cosA;
        x = newX;
        y = newY;
    }

    // Static helpers
    static float angleBetween(const Vector2D& a, const Vector2D& b) {
        return std::atan2(b.y - a.y, b.x - a.x);
    }

    static Vector2D fromAngle(float angleRadians, float length = 1.0f) {
        return Vector2D(std::cos(angleRadians) * length, std::sin(angleRadians) * length);
    }
};
