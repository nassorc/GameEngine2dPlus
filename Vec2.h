#pragma once
#include <cmath>

class Vec2 {
public:
  float x = 0;
  float y = 0;

  Vec2();
  Vec2(float x, float y);

  Vec2 operator + (const Vec2& rhs);
  Vec2 operator - (const Vec2& rhs);
  Vec2 operator * (float value);
  Vec2 operator / (float value);

  Vec2& operator += (const Vec2& rhs);
  Vec2& operator -= (const Vec2& rhs);
  Vec2& operator *= (float value);
  Vec2& operator /= (float value);

  bool operator == (const Vec2& rhs) const;

  Vec2 normalize(float magnitude);
  float magnitude(const Vec2 &rhs);
    float distance();
  Vec2 scale(float scalar);
  Vec2 cross(const Vec2& rhs);
};