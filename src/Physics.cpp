#include "Physics.h"

#include "Components.h"
#include "Entity.h"
#include <cmath>

Physics::Physics() {

}

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    Vec2 overlap;

    auto &posA = a->getComponent<CTransform>().pos;
    auto &posB = b->getComponent<CTransform>().pos;
    auto &hSizeA = a->getComponent<CBoundingBox>().halfSize;
    auto &hSizeB = b->getComponent<CBoundingBox>().halfSize;

    // distance between entity a and b in the x and y direction
    Vec2 delta{abs(posA.x - posB.x), abs(posA.y - posB.y)};

    // compute overlap in x-direction and y-direction
    float ox{(hSizeA.x + hSizeB.x) - delta.x};
    float oy{(hSizeA.y + hSizeB.y) - delta.y};

    overlap.x = ox;
    overlap.y = oy;

    return overlap;
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    // HINT: use definiton of GetOVerlap but use prevPosition instead
    // TODO: return the previous overlap rectangle size of the bounding boxes of entity a and b
    //       previous overlap uses the entity's previous position
    Vec2 overlap;

    auto &posA = a->getComponent<CTransform>().prevPos;
    auto &posB = b->getComponent<CTransform>().pos;
    auto &hSizeA = a->getComponent<CBoundingBox>().halfSize;
    auto &hSizeB = b->getComponent<CBoundingBox>().halfSize;
    // solve dx and dy, distance between entity a and b
    Vec2 delta{abs(posA.x - posB.x), abs(posA.y - posB.y)};

    // compute overlap in x-direction and y-direction
    float ox{(hSizeA.x + hSizeB.x) - delta.x};
    float oy{(hSizeA.y + hSizeB.y) - delta.y};

    overlap.x = ox;
    overlap.y = oy;

    return Vec2(ox, oy);
}

Intersect Physics::LineIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
    Vec2 r = b - a;
    Vec2 s = d - c;
    float rxs = (r.x * s.y) - (r.y * s.x);

    Vec2 cma = c - a;

    float t = ((cma.x * s.y) - (cma.y * s.x)) / rxs;
    float u = ((cma.x * r.y) - (cma.y * r.x)) / rxs;

    if (t>=0 && t<=1 && u>=0 && u<=1) {
        return Intersect{true, Vec2(a.x + t*r.x, a.y + t*r.y)};
    }
    else {
        return Intersect{false, Vec2(0,0)};
    }
}

bool Physics::EntityIntersect(const Vec2& p1, const Vec2& p2, std::shared_ptr<Entity> e) {
    // get all life segments of an entity's bounding box
    auto& eOrigin = e->getComponent<CTransform>().pos;
    auto& eBox = e->getComponent<CBoundingBox>();
    Vec2 e1{eOrigin.x - eBox.halfSize.x, eOrigin.y - eBox.halfSize.y};
    Vec2 e2{eOrigin.x + eBox.halfSize.x, eOrigin.y - eBox.halfSize.y};
    Vec2 e3{eOrigin.x + eBox.halfSize.x, eOrigin.y + eBox.halfSize.y};
    Vec2 e4{eOrigin.x - eBox.halfSize.x, eOrigin.y + eBox.halfSize.y};

//    std::cout << e1.x << " " << e1.y << std::endl;
//    std::cout << eBox.halfSize.x << std::endl;

    Intersect i1 = LineIntersect(p1, p2, e1, e2);
    Intersect i2 = LineIntersect(p1, p2, e2, e3);
    Intersect i3 = LineIntersect(p1, p2, e3, e4);
    Intersect i4 = LineIntersect(p1, p2, e4, e1);

    return i1.result || i2.result || i3.result || i4.result;
}

std::vector<Intersect> Physics::ShapeIntersect(const Vec2 &p1, const Vec2 &p2, std::shared_ptr<Entity> e) {
    auto& shape = e->getComponent<CShape>().shape;
    std::vector<Vec2> points = {};
    std::vector<Intersect> intersects = {};

    // find all line segements of a shape
    for(size_t idx=0; idx < shape.getPointCount(); idx++) {
        Vec2 point(shape.getPoint(idx).x, shape.getPoint(idx).y);
        points.push_back(point);
    }

    // check for intersects
    for(size_t idx=0; idx < points.size(); idx++) {
        size_t nextIdx = (idx + 1) % points.size();

        if (p2 == points[idx] || p2 == points[nextIdx]) { continue;}


//        std::cout << "p1(" << points[idx].x << ", " << points[idx].y << "); " << "p2(" << points[nextIdx].x << ", " << points[nextIdx].y << ") " << std::endl;
//        std::cout << std::endl;
        Intersect intersect = LineIntersect(p1, p2, points[idx], points[nextIdx]);
        if(intersect.result) {
//            std::cout << "p1(" << p1.x << ", " << p1.y << "); " << "p2(" << p2.x << ", " << p2.y << ") " << "I(" << intersect.pos.x << ", " << intersect.pos.y << ") " << std::endl;
//            std::cout << "p1(" << points[idx].x << ", " << points[idx].y << "); " << "p2(" << points[nextIdx].x << ", " << points[nextIdx].y << ") " << std::endl;
//            std::cout << std::endl;
//            std::cout << intersect.pos.x << " " << intersect.pos.y << std::endl;
//            std::cout << intersect.result << std::endl;
            intersects.push_back(intersect);
        }
    }

    // find the closest intersecting point

    return intersects;
}
