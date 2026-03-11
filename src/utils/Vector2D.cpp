//
// Created by wifir on 2026-01-28.
//

#include "Vector2D.h"

#include <complex>
#include <iostream>

Vector2D Vector2D::operator*(const float scalar) const
{
    return {x * scalar, y * scalar};
}

Vector2D operator*(const float scalar, const Vector2D& vector)
{
    return {vector.x * scalar, vector.y * scalar};
}

Vector2D& Vector2D::operator*=(const float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

Vector2D Vector2D::operator-(const Vector2D vector) const
{
    return {x - vector.x, y - vector.y};
}

Vector2D& Vector2D::operator-=(const Vector2D& vector)
{
    this->x -= vector.x;
    this->y -= vector.y;
    return *this;
}

Vector2D Vector2D::operator-() const
{
    return {-x, -y};
}

Vector2D Vector2D::operator+(const Vector2D vector) const
{
    return {x + vector.x, y + vector.y};
}

Vector2D& Vector2D::operator+=(const Vector2D& vector)
{
    this->x += vector.x;
    this->y += vector.y;
    return *this;
}

bool Vector2D::operator==(Vector2D vector) const
{
    return x == vector.x && y == vector.y;
}

bool Vector2D::operator!=(Vector2D vector) const
{
    //return !(this == vector);
    return x != vector.x || y != vector.y;
}

Vector2D& Vector2D::normalize()
{
    if (
        const auto len = static_cast<float>(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));
        len > 0
        )
    {
        this->x /= len;
        this->y /= len;
    }

    return *this;
}

std::string Vector2D::tostring() const
{
    return "{" + std::to_string(this->x) + ", " + std::to_string(this->y) + "}";
}