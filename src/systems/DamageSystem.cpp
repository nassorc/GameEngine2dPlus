//
// Created by Matthew Crossan on 10/6/23.
//

#include "DamageSystem.h"

#include "../components/Components.h"
#include "../core/World.h"
extern World world;

DamageSystem::DamageSystem() : System() {

}

void DamageSystem::update() {
    System::update();
    // damageSystem.add(e1, e2, amount)
    for(auto& e : m_entities) {
//        auto& damageDealer =
        for(auto& collisions : world.get<CCollide1>(e).collisions) {
            auto& collider = collisions.collider;
            if(!world.has<CHealth>(e) || !world.has<CHitbox>(collider)) { continue; }
            auto& health = world.get<CHealth>(e);
            auto& hitbox = world.get<CHitbox>(collider);
        }
    }
}
