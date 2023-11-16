//
// Created by Matthew Crossan on 10/6/23.
//

#include "LifespanSystem.h"

#include "../components/Components.h"
#include "../core/World.h"
extern World world;

LifespanSystem::LifespanSystem() : System() {

}

void LifespanSystem::update() {
    System::update();
    for(auto& e : m_entities) {
        auto& lifespan = world.get<CLifespan>(e);
        if(lifespan.remaining <= 0) {
//            world.removeEntity(e);
//            e->setNotActive();
        }
        else {
//            lifespan.remaining -= 1;
        }
    }
}
