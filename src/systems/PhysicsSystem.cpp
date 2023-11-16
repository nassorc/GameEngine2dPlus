//
// Created by Matthew Crossan on 10/2/23.
//

#include "PhysicsSystem.h"

#include <map>
#include <chrono>
#include "../components/Components.h"
#include "../scenes/SceneBattle.h"
#include "../math/Physics1.h"
#include "../core/World.h"
extern World world;
extern PlayerConfig g_playerConfig;

using namespace std::chrono;


PhysicsSystem::PhysicsSystem() : System() {}

void PhysicsSystem::update() {
    System::update();
    collisionDetection();
    entityCollisionResolution();

}

void PhysicsSystem::collisionDetection() {
    auto start = high_resolution_clock::now();
    for (auto &e1: m_entities) {
        if(!world.has<CMovable>(e1)) { continue; }

        for (auto &e2: m_entities) {
            if(e1->id() == e2->id()) { continue; }

            Vec2 collisionOverlap = Physics1::GetOverlap(e1, e2);

            if(collisionOverlap.x > 0 && collisionOverlap.y > 0) {
                Vec2 collisionPrevOverlap = Physics1::GetPreviousOverlap(e1, e2);
                // CCollide component does not support the new Entity1 version
                if(world.has<CCollide1>(e1)) {
                    world.get<CCollide1>(e1).addCollision(e2, collisionOverlap, collisionPrevOverlap);
                }
                else {
                    world.set<CCollide1>(e1, e2, collisionOverlap, collisionPrevOverlap);
                }

            }
        }


    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
}

void PhysicsSystem::entityCollisionResolution() {
    std::map<size_t, Vec2> collisionMap;

    for (auto &entity: m_entities) {
        if(!world.has<CCollide1>(entity)) { continue; }

        for(auto& collision : world.get<CCollide1>(entity).collisions) {
            auto& overlap = collision.collisionOverlap;
            auto& previousOverlap = collision.previousCollisionOverlap;
            auto e2 = collision.collider;


            if(!world.get<CBoundingBox>(e2).blockMovement) { continue; }

            auto& e1Pos = world.get<CTransform>(entity).pos;
            auto& e2Pos = world.get<CTransform>(e2).pos;

            // TOP COLLISION
            if (previousOverlap.x > 0 && e1Pos.y < e2Pos.y) {
                collisionMap[entity->id()].y = -1 * overlap.y;
                collision.type = Collision1::CollisionType::TOP;

//                if(world.has<CInput>(entity) && !world.get<CInput>(entity).up) {
//                    world.get<CInput>(entity).canJump = true;
//                }
            }
            else if (previousOverlap.x > 0 && e1Pos.y > e2Pos.y) {
                collisionMap[entity->id()].y = overlap.y;
                world.get<CTransform>(entity).velocity.y = 0;
                collision.type = Collision1::CollisionType::BOTTOM;
            }
            else if (previousOverlap.y > 0 && e1Pos.x < e2Pos.x) {
                collisionMap[entity->id()].x = -1 * overlap.x;
                collision.type = Collision1::CollisionType::LEFT;
            }
            else if (previousOverlap.y > 0 && e1Pos.x > e2Pos.x) {
                collisionMap[entity->id()].x = overlap.x;
                collision.type = Collision1::CollisionType::RIGHT;
            }
        }
    }

    for (auto &e1: m_entities) {
        if(collisionMap.find(e1->id()) == collisionMap.end()) { continue; }
        world.get<CTransform>(e1).pos.x += collisionMap[e1->id()].x;
        world.get<CTransform>(e1).pos.y += collisionMap[e1->id()].y;
    }
}
