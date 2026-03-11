//
// Created by wifir on 2026-01-28.
//

#ifndef LECTURE4_VECTOR2D_H
#define LECTURE4_VECTOR2D_H
#include <string>

class Vector2D
{
public:
    float x = 0.0f;
    float y = 0.0f;

    Vector2D()
    {
        x = 0.0f;
        y = 0.0f;
    }

    Vector2D(const float x, const float y) : x(x), y(y) {}

    Vector2D operator*(float scalar) const;
    friend Vector2D operator*(float scalar, const Vector2D& vector);
    Vector2D& operator*=(float scalar);

    Vector2D operator/(float scalar) const;
    friend Vector2D operator/(float scalar, const Vector2D& vector);
    Vector2D operator/=(float scalar);

    Vector2D operator+(Vector2D vector) const;
    Vector2D& operator+=(const Vector2D& vector);

    Vector2D operator-(Vector2D vector) const;
    Vector2D& operator-=(const Vector2D& vector);
    Vector2D operator-()const;

    bool operator==(Vector2D vector) const;
    bool operator!=(Vector2D vector) const;

    Vector2D& normalize();
    std::string tostring() const;
};

#endif //LECTURE4_VECTOR2D_H