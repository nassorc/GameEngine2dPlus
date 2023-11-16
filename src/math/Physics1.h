//
// Created by Matthew Crossan on 10/3/23.
//
#pragma once

#include <cmath>
#include "../components/Components.h"
#include "../core/CoreManager.h"
#include "../core/Entity1.h"
#include "../core/Types.h"


// will access components of entity
class Physics1 {
public:
    Physics1();
    static Vec2 GetOverlap(std::shared_ptr<Entity1> a, std::shared_ptr<Entity1> b);
    static Vec2 GetPreviousOverlap(std::shared_ptr<Entity1> a, std::shared_ptr<Entity1> b);
    // test if two line segments AB and CD intersects, and where
    static Intersect LineIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d);
    // test whether line segment AB intersects with any lines of the bounding box of Entity e
    bool EntityIntersect(const Vec2& p1, const Vec2& p2, std::shared_ptr<Entity1> e);
    static std::vector<Intersect> ShapeIntersect(const Vec2& p1, const Vec2& p2, std::shared_ptr<Entity1> e);
};

