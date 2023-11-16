//
// Created by Matthew Crossan on 10/4/23.
//

#include "CleanupSystem.h"
#include "../components/Components.h"
#include "../scenes/SceneBattle.h"
#include "../core/World.h"
extern World world;

CleanupSystem::CleanupSystem() : System() {

}

void CleanupSystem::update() {
    System::update();
    for (auto &e1: m_entities) {
        if(world.has<CCollide1>(e1)) {
            world.coreManager->removeComponent<CCollide1>(e1);
        }
    }
}