#include "Vec2.h"
#include <cmath>

Vec2::Vec2()
{
  
}

Vec2::Vec2(float x, float y)
  : x(x)
  , y(y)
{
  
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
  return Vec2{x + rhs.x, y + rhs.y};
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
  return Vec2{x - rhs.x, y - rhs.y};
}

Vec2 Vec2::operator * (float value) const
{
  return Vec2{x * value, y * value};
}

Vec2 Vec2::operator / (float value) const
{
  return Vec2{x/value, y/value};
}

Vec2& Vec2::operator += (const Vec2& rhs)
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}

Vec2& Vec2::operator -= (const Vec2& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

Vec2& Vec2::operator *= (float value)
{
    x *= value;
    y *= value;
  return *this;
}

Vec2& Vec2::operator /= (float value)
{
  return *this;
}

Vec2 Vec2::normalize() const
{
    Vec2 v{x, y};
    return v / v.magnitude();
}

//float Vec2::magnitude(const Vec2 &rhs) const
//{
//    return sqrt((rhs.x * x) + (rhs.y * y));
//}

float Vec2::magnitude() const
{
    return sqrt((x * x) + (y * y));
}

Vec2 Vec2::scale(float scalar)
{
  return Vec2(x*scalar, y*scalar);
}

Vec2 Vec2::cross(const Vec2& rhs)
{
  return *this;
}

bool Vec2::operator==(const Vec2 &rhs) const {
    return (x == rhs.x) && (y == rhs.y);
}

float Vec2::distance() {
    return sqrt((x * x) + (y * y));
}

Vec2 Vec2::normalize(const Vec2& v) {
    float m = v.magnitude();
    // divide vector by magnitude
//    Vec2 vdm = v / m;
    return v / m;
}

