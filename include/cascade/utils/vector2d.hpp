#ifndef CASCADE_UTILS_VECTOR2D_HPP
#define CASCADE_UTILS_VECTOR2D_HPP

#include <cmath>

namespace cascade {
    struct Vector2D {
        float x;
        float y;

        Vector2D() : x(0.0f), y(0.0f) {}
        Vector2D(float x, float y) : x(x), y(y) {}

        Vector2D operator+(const Vector2D& other) const {
            return Vector2D(x + other.x, y+ other.y);
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

        bool operator==(const Vector2D& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vector2D& other) const {
            return !(*this == other);
        }

        float length() const {
            return std::sqrt(x*x + y*y);
        }

        float lengthSquared() const {
            return x * x + y * y;
        }

        Vector2D normalized() const {
            float len = length();
            if (len > 0.0f) {
                return *this / len;
            }
            return Vector2D(0.0f, 0.0f);
        }

        void normalize() {
            float len = length();
            if (len > 0.0f) {
                x /= len;
                y /= len;
            }
        }

        float dot(const Vector2D& other) const {
            return x * other.x +  y * other.y;
        }

        float distance(const Vector2D& other) const {
            return (*this - other).length();
        }

        float distanceSquared(const Vector2D& other) const {
            return (*this - other).lengthSquared();
        }

        static float distance(const Vector2D& a, const Vector2D& b) {
            return a.distance(b);
        }

        static float dot(const Vector2D& a, const Vector2D& b) {
            return a.dot(b);
        }
    };

    inline Vector2D operator*(float scalar, const Vector2D& vec) {
        return vec * scalar;
    }
}

#endif