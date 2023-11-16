#pragma once

#include "../components/Components.h"
#include "Entity.h"
#include <cmath>
#include "../core/Types.h"

// will access components of entity
class Physics {
public:
    Physics();
    Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    // test if two line segments AB and CD intersects, and where
    Intersect LineIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d);
    // test whether line segment AB intersects with any lines of the bounding box of Entity e
    bool EntityIntersect(const Vec2& p1, const Vec2& p2, std::shared_ptr<Entity> e);
    std::vector<Intersect> ShapeIntersect(const Vec2& p1, const Vec2& p2, std::shared_ptr<Entity> e);
};

