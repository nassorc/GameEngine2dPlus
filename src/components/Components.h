#pragma once

#include <memory>
#include "../core/Animation.h"
#include "queue"
#include "../math/Vec2.h"

#include "SFML/Graphics.hpp"

class Entity;
class Entity1;

enum StateType {
    IDLE,
    RUNNING,
    JUMPING,
    ATTACKING,
    HIT,
};
enum Direction {
    LEFT = -1,
    RIGHT = 1
};

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
    Direction direction = Direction::RIGHT;
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
    CText() {
        text = sf::Text();
    }
    CText(sf::Text text)
        : text(text) {
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
    bool canWallJump = false;
    bool isGrounded = false;

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
    CHealth(int total)
        : total(total)
        , remaining(total)
    {}
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
    bool blockMovement;
    bool blockVision;

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
            , offset(o)
            , blockMovement(bm)
            , blockVision(bv)
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


// CState will determine the state and animation of the entity
class CState : public Component {
public:
    StateType state = StateType::IDLE;

    CState() { }
    CState(StateType s) : state(s) {
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

class CDialogue : public Component {
public:
    std::queue<std::string> dialogues;
    std::string npcName;
    size_t currentCharIdx = 1;
    bool active = false;
    CDialogue() {
        dialogues.push("hello world");
        dialogues.push("good bye");
        npcName = "Shopkeeper";
    }
};

class CDialogue1 : public Component {
public:
    std::string name;
    std::string dialoguePath;
    size_t currentDialogue = 0;
    size_t currentSentence = 0;
    bool active = false;
    CDialogue1() {}
    CDialogue1(const std::string& path, const std::string& name, int currentDialogue, int currentSentence)
        : dialoguePath(path)
        , name(name)
        , currentDialogue(currentDialogue)
        , currentSentence(currentSentence)
    {}
};

struct Collision {
    enum CollisionType {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        NONE
    };
    std::shared_ptr<Entity> collider;
    Vec2 collisionOverlap;
    Vec2 previousCollisionOverlap;
    CollisionType type;
};

class CFrameCollision : public Component {
public:
    Collision collision;
    CFrameCollision() {}
    CFrameCollision(std::shared_ptr<Entity> collider, const Vec2& overlap, const Vec2& previousOverlap) {
        collision = Collision {collider, overlap, previousOverlap};
    }
//    void addCollision(std::shared_ptr<Entity> collider, const Vec2& overlap, const Vec2& previousOverlap) {
//        collisions.push_back(Collision {collider, overlap, previousOverlap});
//    }
};

class CMovable : public Component {
public:
    CMovable() {}
};

//class CHitbox : public Component {
//public:
//    std::shared_ptr<Entity1> source;
//    Vec2 size;
//    Vec2 offset;
//    float damage;
//    bool active;
//
//    CHitbox() { }
//    CHitbox(std::shared_ptr<Entity1> source, const Vec2& size, const Vec2& offset, float damage = 1, bool active = false)
//        : source(source)
//        , size(size)
//        , offset(offset)
//        , damage(damage)
//        , active(active) { }
//};

enum HitboxBehavior {
    Static,
    Follow
};
class CHitbox : public Component {
public:
    std::shared_ptr<Entity1> source;
    HitboxBehavior behavior;
    bool active;

    CHitbox() { }
    CHitbox(std::shared_ptr<Entity1> source, bool active = false, bool followSource = true)
            : source(source)
            , active(active)
    {
        behavior = (followSource) ? HitboxBehavior::Follow : HitboxBehavior::Static;
    }
};

class CRect : public Component {
public:
    sf::RectangleShape rect;
    CRect() {}
    CRect(Vec2 size) {
        rect.setSize(sf::Vector2f(size.x, size.y));
        rect.setOrigin(size.x / 2, size.y / 2);
    }
    CRect(float width, float height) {
        rect.setSize(sf::Vector2f{width,  height});
    }
};

class CCollide : public Component {
public:
    std::vector<Collision> collisions;
    CCollide() {}
    CCollide(std::shared_ptr<Entity> collider, const Vec2& overlap, const Vec2& previousOverlap) {
        addCollision(collider, overlap, previousOverlap);
    }
    void addCollision(std::shared_ptr<Entity> collider, const Vec2& overlap, const Vec2& previousOverlap) {
        collisions.push_back(Collision{collider, overlap, previousOverlap});
    }
};

// where the collision occured
// type of collision (hit/interact/boundingbox)

struct Collision1 {
    enum CollisionType {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        NONE
    };
    enum CollisionResponse {
        DAMAGE,
        INTERACT,
    };
    std::shared_ptr<Entity1> collider;
    Vec2 collisionOverlap;
    Vec2 previousCollisionOverlap;
    CollisionType type;
};

class CCollide1 : public Component {
public:
    std::vector<Collision1> collisions;
    CCollide1() {}
    CCollide1(std::shared_ptr<Entity1> collider, const Vec2& overlap, const Vec2& previousOverlap) {
        addCollision(collider, overlap, previousOverlap);
    }
    void addCollision(std::shared_ptr<Entity1> collider, const Vec2& overlap, const Vec2& previousOverlap) {
        collisions.push_back(Collision1{collider, overlap, previousOverlap});
    }
};

class CDrag : public Component {
public:
    Vec2 drag{0,0};
    CDrag() {}
    CDrag(float x, float y) : drag(x, y) {}
};

class CAI : public Component {
public:
    CAI() {}
};

class CAISeekBehavior : public Component {
public:
    CAISeekBehavior() {}
};

class CAIFlying : public Component {
public:
    CAIFlying() {}
};

enum SpellType {
    FIREBALL,
    HEAL
};

class CSpellCaster : public Component {
public:
    SpellType spellType;
    std::shared_ptr<Entity1> caster;
    CSpellCaster() {}
    CSpellCaster(SpellType spellType, std::shared_ptr<Entity1> caster)
        : spellType(spellType)
        , caster(caster) {}
};

class CPlayer : public Component {
public:
    CPlayer() {}
};

// prefab<type, fn>
//    FIREBALL: createFireball

// if spellcaster.type == spelltype
//      ecs.prefab(spellcaster.type) // creates a predefined entity
//

//struct Collision {
//    std::shared_ptr<Entity> collider;
//    Vec2 collisionOverlap;
//    Vec2 previousCollisionOverlap;
//};

//enum PlayerState {
//    PLAYER_RUNNING,
//    PLAYER_JUMPING,
//    PLAYER_GROUNDED,
//    PLAYER_ATTACKING,
//};
//
//struct CPlayerState {
//    CPlayerState() {}
//};

enum EnemyState {
    ENEMY_BEHAVIOR,
    FOLLOW_PLAYER,
    ATTACK_PLAYER,
};

enum AttackType {
    SHOOT,
    MELEE,
    CAST_NM_SPELL
};
// STATE SYSTEM
// if state == attack
//      e->set<Attack>(ref)
// ATTACK SYSTEM
//

struct CProjectTile {
    // source
};

struct CStatusEffect {

};

struct CAttack {
    std::string type;  // MELEEE
                       // REFERENCE  if(normalENTITY) type == "attack_lung"
};
// if type == "shoot_projectile" { e->set<CProjectile>() }

struct CEnemyState {
    EnemyState state;
    // ref = "normalFlyer"
    /*
     * { "normalFlyer" : {
     *      "attack": "CAST_"
     * }}
     *
     * ""
     */

    CEnemyState () {}
};

//class AttackSystem {
//public:
//    AttackSystem() {}
//    void update() {
//        for (auto e : m_entities) {
//            auto& attackType = world.get<CAttack>(e);
//            if(attackType == "melee_lunge") {
//                world.set<CMeleeAttack>(e);
//                world.set<CLunge>(e);
//            }
//            else if(attackType == "melee_swing") {
//                world.set<CMeleeAttack>(e);
//                world.set<CSwing>(e);
//            }
//        }
//    }
//};
//class MeleeAttackSystem {
//public:
//    MeleeAttackSystem() {}
//    void update() {
//        for (auto e : m_entities) {
//            if(world.has<CLungeAttack>(e)) {
//                // perform lunge attack
//            }
//            else if(world.has<CSwingAttack>(e)) {
//                // perform swing attack
//            }
//        }
//    }
//};
