#pragma once

#include "Animation.h"
#include "Vec2.h"

#include "SFML/Graphics.hpp"

class Component {
public:
    bool has = false;

    Component() {}
};

class CParallax : public Component {
public:
    Vec2 parallax{1, 1};
    CParallax () {}
    CParallax (float x, float y) : parallax(Vec2{x, y}) {
        this->has = true;
    }
};

class CZIndex : public Component {
    int zIndex = 1;
    CZIndex() {}
    CZIndex(int idx) : zIndex(idx) {
        this->has = true;
    }
};

// contains data on where an entities is, how fast it's going, and the rotation
class CTransform : public Component {
public:
    Vec2 pos = {0.0, 0.0};
    Vec2 prevPos = {0.0, 0.0};  // position of the entity on the previous frame
    Vec2 scale = {1.0, 1.0};
    Vec2 velocity = {0.0, 0.0};
    float angle = 0;

    CTransform() {}

    CTransform(const Vec2 &p, const Vec2 &v, float a)
            : pos(p), velocity(v), angle(a) {
        this->has = true;
    }
};

class CText : public Component {
public:
    sf::Text text;

    CText() {}

    CText(sf::Text text)
            : text(text) {
        this->has = true;
    }
};

class CCollision : public Component {
public:
    float radius = 0;

    CCollision(float r) : radius(r) {
        this->has = true;
    }
};

class CInput : public Component {
public:
    bool up = false;
    bool left = false;
    bool right = false;
    bool down = false;
    bool attack = false;
    bool canAttack = true;
    bool canJump = true;

    CInput() {
        this->has = true;
    }
};

class CLifespan : public Component {
    // every frame we remove from remaining
public:
    int remaining = 0;  // amount of lifespan remaining on the entity
    int total = 0;  // the total initial amount of lifespan
    CLifespan() {
    }
    CLifespan(int total)
        : remaining(total)
        , total(total) {
        this->has = true;
    }
};

class CHealth : public Component {
    // every frame we remove from remaining
public:
    int total = 0;  // the total initial amount of lifespan
    int remaining = 0;  // amount of lifespan remaining on the entity
    CHealth() {
    }
    CHealth(int total, int remaining)
            : total(total)
            , remaining(remaining) {
        this->has = true;
    }
};

class CScore : public Component {
public:
    CScore() {}
};

class CBoundingBox : public Component {
public:
    Vec2 size {0.0f, 0.0f};
    Vec2 halfSize {0.0f, 0.0f};
    Vec2 offset {0.0f, 0.0f};
    bool blockMovement = false;
    bool blockVision = false;

    CBoundingBox() {}
    CBoundingBox(const Vec2 &s, bool bm, bool bv)
        : size(s)
        , halfSize(s.x / 2, s.y / 2)
        , blockMovement(bm)
        , blockVision(bv)
    {
        has = true;
    }
    CBoundingBox(const Vec2 &s, const Vec2 &o, bool bm, bool bv)
            : size(s)
            , halfSize(s.x / 2, s.y / 2)
            , blockMovement(bm)
            , blockVision(bv)
            , offset(o)
    {
        has = true;
    }

};

class CAnimation : public Component {
public:
    Animation animation;
    bool repeat = false;

    CAnimation() {}

    CAnimation(Animation &a) : animation(a) {
        has = true;
    }
};

class CSprite : public Component {
public:
    sf::Sprite sprite;

    CSprite() {}
    CSprite(sf::Sprite &sprite) : sprite(sprite) {
        has = true;
    }
};


class CGravity : public Component {
    // gravity is added to the velocity
    // ctransform.velocity.y += gravity
public:
    float gravity = 0;

    CGravity() {}

    CGravity(float g)
            : gravity(g) {
        has = true;
    }
};

enum PlayerState {
    IDLE,
    RUNNING,
    JUMPING,
    ATTACKING,
    HIT
};

// CState will determine the state and animation of the entity
class CState : public Component {
public:
    std::string state = "STANDING";

    CState() {
        this->has = true;
    }

    CState(const std::string &s) : state(s) {
        this->has = true;
    }
};

class CDraggable : public Component {
public:
    bool dragging = false;

    CDraggable() {}

    CDraggable(bool d)
            : dragging(d) {
        has = true;
    }
};

class CDamage : public Component {
public:
    int damage = 1;
    CDamage(){}
    CDamage(int d) : damage(d) {
        this->has = true;
    }
};

class CInvincibility : public Component {
public:
    int iFrames = 0;
    CInvincibility(){}
    CInvincibility(int f) : iFrames(f) {
        this->has = true;
    }
};

class CFollowPlayer : public Component {
public:
    Vec2 home = {0,0};
    float speed = 0;
    CFollowPlayer(){
    }
    CFollowPlayer(Vec2 p, float s) : home(p), speed(s) {
        this->has = true;
    }
};

class CPatrol : public Component{
public:
    std::vector<Vec2> positions;
    size_t currentPosition = 0;
    float speed = 0;
    CPatrol() {
    }
    CPatrol(std::vector<Vec2>& pos, float s) : positions(pos), speed(s) {
        this->has = true;
    }
};

class CShape : public Component{
public:
    sf::ConvexShape shape;
    CShape() {}
    CShape (std::vector<Vec2>& points, const sf::Color& color) {
        this->has = true;
        shape.setPointCount(points.size());
        shape.setOutlineThickness(3);
        shape.setOutlineColor(sf::Color(170, 170, 170));
        shape.setFillColor(color);
        for (size_t idx=0; idx < points.size(); idx++) {
            shape.setPoint(idx, sf::Vector2f(points[idx].x, points[idx].y));
        }
    }
};
